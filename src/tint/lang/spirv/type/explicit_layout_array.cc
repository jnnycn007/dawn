// Copyright 2025 The Dawn & Tint Authors
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

#include "src/tint/lang/spirv/type/explicit_layout_array.h"

#include <string>

#include "src/tint/lang/core/type/manager.h"
#include "src/tint/utils/math/hash.h"
#include "src/tint/utils/text/string_stream.h"

TINT_INSTANTIATE_TYPEINFO(tint::spirv::type::ExplicitLayoutArray);

namespace tint::spirv::type {

ExplicitLayoutArray::ExplicitLayoutArray(const Type* element,
                                         const core::type::ArrayCount* count,
                                         uint32_t align,
                                         uint32_t size,
                                         uint32_t stride)
    : Base(Hash(tint::TypeCode::Of<ExplicitLayoutArray>().bits, count),
           element,
           count,
           align,
           size,
           stride) {}

bool ExplicitLayoutArray::Equals(const UniqueNode& other) const {
    if (other.Is<ExplicitLayoutArray>()) {
        return Array::Equals(other);
    }
    return false;
}

std::string ExplicitLayoutArray::FriendlyName() const {
    StringStream out;
    out << "spirv.explicit_layout_array<" << element_->FriendlyName();
    if (!count_->Is<core::type::RuntimeArrayCount>()) {
        out << ", " << count_->FriendlyName();
    }
    out << ", stride=" << stride_ << ">";
    return out.str();
}

ExplicitLayoutArray* ExplicitLayoutArray::Clone(core::type::CloneContext& ctx) const {
    auto* elem_ty = element_->Clone(ctx);
    auto* count = count_->Clone(ctx);
    return ctx.dst.mgr->Get<ExplicitLayoutArray>(elem_ty, count, align_, size_, stride_);
}

}  // namespace tint::spirv::type
