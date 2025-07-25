// Copyright 2023 The Dawn & Tint Authors
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_DAWN_NATIVE_EXECUTIONQUEUE_H_
#define SRC_DAWN_NATIVE_EXECUTIONQUEUE_H_

#include <atomic>
#include <functional>

#include "dawn/common/MutexProtected.h"
#include "dawn/common/SerialMap.h"
#include "dawn/native/Error.h"
#include "dawn/native/IntegerTypes.h"
#include "partition_alloc/pointers/raw_ptr.h"

namespace dawn::native {

// Represents an engine which processes a stream of GPU work. It handles the tracking and
// update of the various ExecutionSerials related to that work.
// TODO(dawn:831, dawn:1413): Make usage of the ExecutionQueue thread-safe. Right now it is
// only partially safe - where observation of the last-submitted and pending serials is atomic.
class ExecutionQueueBase {
  public:
    using Task = std::function<void()>;

    // The latest serial known to have completed execution on the queue.
    ExecutionSerial GetCompletedCommandSerial() const;
    // The serial of the latest batch of work sent for execution.
    ExecutionSerial GetLastSubmittedCommandSerial() const;
    // The serial of the batch that is currently pending submission.
    ExecutionSerial GetPendingCommandSerial() const;
    // The serial by which time all currently submitted or pending operations will be completed.
    ExecutionSerial GetScheduledWorkDoneSerial() const;
    // Whether the execution queue has scheduled commands to be submitted or executing.
    bool HasScheduledCommands() const;

    // Check for passed fences and set the new completed serial. Note that the two functions below
    // effectively do the same thing initially, however, |UpdateCompletedSerials| requires
    // that the device-wide lock is NOT held since it may additionally trigger user callbacks. Note
    // that for the purpose of going forwards, |CheckPassedSerials| should not be used anymore since
    // it assumes that the device-wide lock IS held.
    // TODO(crbug.com/42240396): Remove |CheckPassedSerials| in favor of |UpdateCompletedSerial|.
    MaybeError CheckPassedSerials();
    MaybeError UpdateCompletedSerial();

    // For the commands being internally recorded in backend, that were not urgent to submit, this
    // method makes them to be submitted as soon as possible in next ticks.
    virtual void ForceEventualFlushOfCommands() = 0;

    // Ensures that all commands which were recorded are flushed upto the given serial.
    MaybeError EnsureCommandsFlushed(ExecutionSerial serial);

    // Submit any pending commands that are enqueued.
    MaybeError SubmitPendingCommands();

    // During shut down of device, some operations might have been started since the last submit
    // and waiting on a serial that doesn't have a corresponding fence enqueued. Fake serials to
    // make all commands look completed.
    void AssumeCommandsComplete();

    // Increment mLastSubmittedSerial when we submit the next serial
    void IncrementLastSubmittedCommandSerial();

    // WaitForIdleForDestruction waits for GPU to finish, checks errors and gets ready for
    // destruction. This is only used when properly destructing the device. For a real
    // device loss, this function doesn't need to be called since the driver already closed all
    // resources.
    virtual MaybeError WaitForIdleForDestruction() = 0;

    // Wait at most `timeout` synchronously for the ExecutionSerial to pass. Returns true
    // if the serial passed.
    ResultOrError<bool> WaitForQueueSerial(ExecutionSerial serial, Nanoseconds timeout);

    // Tracks a new task to complete when |serial| is reached.
    void TrackSerialTask(ExecutionSerial serial, Task&& task);

    // In the 'Normal' mode, currently recorded commands in the backend submitted in the next Tick.
    // However in the 'Passive' mode, the submission will be postponed as late as possible, for
    // example, until the client has explictly issued a submission.
    enum class SubmitMode { Normal, Passive };

    // Tracks whether we are in a submit to avoid submit reentrancy. Reentrancy could otherwise
    // happen when allocating resources or staging memory during submission (for workarounds, or
    // emulation) and the heuristics ask for an early submit to happen (which would cause a
    // submit-in-submit and many issues).
    // TODO(crbug.com/42240396): Move all handling of Submit(command buffers) in this class as well,
    // at which point this member can be private.
    bool mInSubmit = false;

  protected:
    // Currently, the queue has two paths for serial updating, one is via DeviceBase::Tick which
    // calls into the backend specific polling mechanisms implemented in
    // CheckAndUpdateCompletedSerials. Alternatively, the backend can actively call
    // UpdateCompletedSerial when a new serial is complete to make forward progress proactively.
    void UpdateCompletedSerialTo(ExecutionSerial completedSerial);

    // Backend specific wait function.
    virtual ResultOrError<bool> WaitForQueueSerialImpl(ExecutionSerial serial,
                                                       Nanoseconds timeout) = 0;

  private:
    // Each backend should implement to check their passed fences if there are any and return a
    // completed serial. Return 0 should indicate no fences to check.
    virtual ResultOrError<ExecutionSerial> CheckAndUpdateCompletedSerials() = 0;
    // mCompletedSerial tracks the last completed command serial that the fence has returned.
    // mLastSubmittedSerial tracks the last submitted command serial.
    // During device removal, the serials could be artificially incremented
    // to make it appear as if commands have been compeleted.
    std::atomic<uint64_t> mCompletedSerial = static_cast<uint64_t>(kBeginningOfGPUTime);
    std::atomic<uint64_t> mLastSubmittedSerial = static_cast<uint64_t>(kBeginningOfGPUTime);

    MutexProtected<SerialMap<ExecutionSerial, Task>> mWaitingTasks;

    // Indicates whether the backend has pending commands to be submitted as soon as possible.
    virtual bool HasPendingCommands() const = 0;

    // Submit any pending commands that are enqueued.
    virtual MaybeError SubmitPendingCommandsImpl() = 0;
};

}  // namespace dawn::native

#endif  // SRC_DAWN_NATIVE_EXECUTIONQUEUE_H_
