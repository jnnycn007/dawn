// Copyright 2021 The Dawn & Tint Authors
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

#include <gtest/gtest.h>
#include <unordered_set>

#include "dawn/common/Constants.h"
#include "dawn/native/ChainUtils.h"
#include "dawn/native/Limits.h"

namespace dawn {
namespace native {

// Test |GetDefaultLimits| returns the default for FeatureLeveL::Core.
TEST(Limits, GetDefaultLimits) {
    CombinedLimits limits = {};
    EXPECT_NE(limits.v1.maxBindGroups, 4u);

    GetDefaultLimits(&limits, wgpu::FeatureLevel::Core);

    EXPECT_EQ(limits.v1.maxBindGroups, 4u);
}

// Test |GetDefaultLimits| returns the default for FeatureLevel::Compatibility.
// Compatibility default limits are lower than Core.
TEST(Limits, GetDefaultLimits_Compat) {
    CombinedLimits limits = {};
    EXPECT_NE(limits.v1.maxColorAttachments, 4u);

    GetDefaultLimits(&limits, wgpu::FeatureLevel::Compatibility);

    EXPECT_EQ(limits.v1.maxColorAttachments, 4u);
}

// Test |ReifyDefaultLimits| populates the default for wgpu::FeatureLevel::Core
// if values are undefined.
TEST(Limits, ReifyDefaultLimits_PopulatesDefault) {
    CombinedLimits limits;
    limits.v1.maxComputeWorkgroupStorageSize = wgpu::kLimitU32Undefined;
    limits.v1.maxStorageBufferBindingSize = wgpu::kLimitU64Undefined;

    CombinedLimits reified = ReifyDefaultLimits(limits, wgpu::FeatureLevel::Core);
    EXPECT_EQ(reified.v1.maxComputeWorkgroupStorageSize, 16384u);
    EXPECT_EQ(reified.v1.maxStorageBufferBindingSize, 134217728ul);
    EXPECT_EQ(reified.compat.maxStorageBuffersInFragmentStage, 8u);
    EXPECT_EQ(reified.compat.maxStorageTexturesInFragmentStage, 4u);
    EXPECT_EQ(reified.compat.maxStorageBuffersInVertexStage, 8u);
    EXPECT_EQ(reified.compat.maxStorageTexturesInVertexStage, 4u);
}

// Test |ReifyDefaultLimits| populates the default for wgpu::FeatureLevel::Compatibility
// if values are undefined. Compatibility default limits are lower than Core.
TEST(Limits, ReifyDefaultLimits_PopulatesDefault_Compat) {
    CombinedLimits limits;
    limits.v1.maxTextureDimension1D = wgpu::kLimitU32Undefined;
    limits.v1.maxStorageBufferBindingSize = wgpu::kLimitU64Undefined;

    CombinedLimits reified = ReifyDefaultLimits(limits, wgpu::FeatureLevel::Compatibility);
    EXPECT_EQ(reified.v1.maxTextureDimension1D, 4096u);
    EXPECT_EQ(reified.v1.maxStorageBufferBindingSize, 134217728ul);
    EXPECT_EQ(reified.compat.maxStorageBuffersInFragmentStage, 4u);
    EXPECT_EQ(reified.compat.maxStorageTexturesInFragmentStage, 4u);
    EXPECT_EQ(reified.compat.maxStorageBuffersInVertexStage, 0u);
    EXPECT_EQ(reified.compat.maxStorageTexturesInVertexStage, 0u);
}

// Test |ReifyDefaultLimits| clamps to the default if
// values are worse than the default.
TEST(Limits, ReifyDefaultLimits_Clamps) {
    CombinedLimits limits;
    limits.v1.maxStorageBuffersPerShaderStage = 4;
    limits.v1.minUniformBufferOffsetAlignment = 512;

    CombinedLimits reified = ReifyDefaultLimits(limits, wgpu::FeatureLevel::Core);
    EXPECT_EQ(reified.v1.maxStorageBuffersPerShaderStage, 8u);
    EXPECT_EQ(reified.v1.minUniformBufferOffsetAlignment, 256u);
}

// Test |ValidateLimits| works to validate limits are not better
// than supported.
TEST(Limits, ValidateLimits) {
    const wgpu::FeatureLevel featureLevel = wgpu::FeatureLevel::Core;
    // Start with the default for supported.
    CombinedLimits defaults;
    GetDefaultLimits(&defaults, featureLevel);

    // Test supported == required is valid.
    {
        CombinedLimits required = defaults;
        EXPECT_TRUE(ValidateLimits(defaults, required).IsSuccess());
    }

    // Test supported == required is valid, when they are not default.
    {
        CombinedLimits supported = defaults;
        CombinedLimits required = defaults;
        supported.v1.maxBindGroups += 1;
        required.v1.maxBindGroups += 1;
        EXPECT_TRUE(ValidateLimits(supported, required).IsSuccess());
    }

    // Test that default-initialized (all undefined) is valid.
    {
        CombinedLimits required = {};
        EXPECT_TRUE(ValidateLimits(defaults, required).IsSuccess());
    }

    // Test that better than supported is invalid for "maximum" limits.
    {
        CombinedLimits required = {};
        required.v1.maxTextureDimension3D = defaults.v1.maxTextureDimension3D + 1;
        MaybeError err = ValidateLimits(defaults, required);
        EXPECT_TRUE(err.IsError());
        err.AcquireError();
    }

    // Test that worse than supported is valid for "maximum" limits.
    {
        CombinedLimits required = {};
        required.v1.maxComputeWorkgroupSizeX = defaults.v1.maxComputeWorkgroupSizeX - 1;
        EXPECT_TRUE(ValidateLimits(defaults, required).IsSuccess());
    }

    // Test that better than min is invalid for "alignment" limits.
    {
        CombinedLimits required = {};
        required.v1.minUniformBufferOffsetAlignment =
            defaults.v1.minUniformBufferOffsetAlignment / 2;
        MaybeError err = ValidateLimits(defaults, required);
        EXPECT_TRUE(err.IsError());
        err.AcquireError();
    }

    // Test that worse than min and a power of two is valid for "alignment" limits.
    {
        CombinedLimits required = {};
        required.v1.minStorageBufferOffsetAlignment =
            defaults.v1.minStorageBufferOffsetAlignment * 2;
        EXPECT_TRUE(ValidateLimits(defaults, required).IsSuccess());
    }

    // Test that worse than min and not a power of two is invalid for "alignment" limits.
    {
        CombinedLimits required = {};
        required.v1.minStorageBufferOffsetAlignment =
            defaults.v1.minStorageBufferOffsetAlignment * 3;
        MaybeError err = ValidateLimits(defaults, required);
        EXPECT_TRUE(err.IsError());
        err.AcquireError();
    }
}

// Basic test of |ValidateAndUnpackLimitsIn|.
TEST(Limits, ValidateAndUnpackLimitsIn) {
    {
        CompatibilityModeLimits requiredCompat{};
        requiredCompat.maxStorageBuffersInFragmentStage = 1234u;

        Limits required{.nextInChain = &requiredCompat};

        CombinedLimits combined{};
        EXPECT_TRUE(ValidateAndUnpackLimitsIn(&required, {}, &combined).IsSuccess());
        EXPECT_EQ(combined.v1.maxStorageBuffersPerShaderStage, wgpu::kLimitU32Undefined);
        EXPECT_EQ(combined.compat.maxStorageBuffersInFragmentStage, 1234u);
    }
}

// Test that |ApplyLimitTiers| degrades limits to the next best tier.
TEST(Limits, ApplyLimitTiers) {
    auto SetLimitsStorageBufferBindingSizeTier2 = [](CombinedLimits* limits) {
        // Tier 2 of maxStorageBufferBindingSize is 1GB
        limits->v1.maxStorageBufferBindingSize = 1073741824;
        // Also set the maxBufferSize to be large enough, as ApplyLimitTiers ensures tired
        // maxStorageBufferBindingSize no larger than tiered maxBufferSize.
        limits->v1.maxBufferSize = 2147483648;
    };
    CombinedLimits limitsStorageBufferBindingSizeTier2;
    GetDefaultLimits(&limitsStorageBufferBindingSizeTier2, wgpu::FeatureLevel::Core);
    SetLimitsStorageBufferBindingSizeTier2(&limitsStorageBufferBindingSizeTier2);

    auto SetLimitsStorageBufferBindingSizeTier3 = [](CombinedLimits* limits) {
        // Tier 3 of maxStorageBufferBindingSize is 2GB-4
        limits->v1.maxStorageBufferBindingSize = 2147483644;
        // Also set the maxBufferSize to be large enough, as ApplyLimitTiers ensures tired
        // maxStorageBufferBindingSize no larger than tiered maxBufferSize.
        limits->v1.maxBufferSize = 2147483648;
    };
    CombinedLimits limitsStorageBufferBindingSizeTier3;
    GetDefaultLimits(&limitsStorageBufferBindingSizeTier3, wgpu::FeatureLevel::Core);
    SetLimitsStorageBufferBindingSizeTier3(&limitsStorageBufferBindingSizeTier3);

    auto SetLimitsComputeWorkgroupStorageSizeTier1 = [](CombinedLimits* limits) {
        limits->v1.maxComputeWorkgroupStorageSize = 16384;
    };
    CombinedLimits limitsComputeWorkgroupStorageSizeTier1;
    GetDefaultLimits(&limitsComputeWorkgroupStorageSizeTier1, wgpu::FeatureLevel::Core);
    SetLimitsComputeWorkgroupStorageSizeTier1(&limitsComputeWorkgroupStorageSizeTier1);

    auto SetLimitsComputeWorkgroupStorageSizeTier3 = [](CombinedLimits* limits) {
        limits->v1.maxComputeWorkgroupStorageSize = 65536;
    };
    CombinedLimits limitsComputeWorkgroupStorageSizeTier3;
    GetDefaultLimits(&limitsComputeWorkgroupStorageSizeTier3, wgpu::FeatureLevel::Core);
    SetLimitsComputeWorkgroupStorageSizeTier3(&limitsComputeWorkgroupStorageSizeTier3);

    // Test that applying tiers to limits that are exactly
    // equal to a tier returns the same values.
    {
        CombinedLimits combinedLimits = {};
        combinedLimits = limitsStorageBufferBindingSizeTier2;
        EXPECT_EQ(ApplyLimitTiers(combinedLimits).v1, combinedLimits.v1);

        combinedLimits = limitsStorageBufferBindingSizeTier3;
        EXPECT_EQ(ApplyLimitTiers(combinedLimits).v1, combinedLimits.v1);
    }

    // Test all limits slightly worse than tier 3.
    {
        CombinedLimits combinedLimits = {};
        combinedLimits = limitsStorageBufferBindingSizeTier3;
        combinedLimits.v1.maxStorageBufferBindingSize -= 1;
        EXPECT_EQ(ApplyLimitTiers(combinedLimits).v1, limitsStorageBufferBindingSizeTier2.v1);
    }

    // Test that limits may match one tier exactly and be degraded in another tier.
    // Degrading to one tier does not affect the other tier.
    {
        CombinedLimits combinedLimits = {};
        combinedLimits = limitsComputeWorkgroupStorageSizeTier3;
        // Set tier 3 and change one limit to be insufficent.
        SetLimitsStorageBufferBindingSizeTier3(&combinedLimits);
        combinedLimits.v1.maxStorageBufferBindingSize -= 1;

        CombinedLimits tiered = ApplyLimitTiers(combinedLimits);

        // Check that |tiered| has the limits of memorySize tier 2
        CombinedLimits tieredWithMemorySizeTier2 = tiered;
        SetLimitsStorageBufferBindingSizeTier2(&tieredWithMemorySizeTier2);
        EXPECT_EQ(tiered.v1, tieredWithMemorySizeTier2.v1);

        // Check that |tiered| has the limits of bindingSpace tier 3
        CombinedLimits tieredWithBindingSpaceTier3 = tiered;
        SetLimitsComputeWorkgroupStorageSizeTier3(&tieredWithBindingSpaceTier3);
        EXPECT_EQ(tiered.v1, tieredWithBindingSpaceTier3.v1);
    }

    // Test that limits may be simultaneously degraded in two tiers independently.
    {
        CombinedLimits limits = {};
        GetDefaultLimits(&limits, wgpu::FeatureLevel::Core);
        SetLimitsComputeWorkgroupStorageSizeTier3(&limits);
        SetLimitsStorageBufferBindingSizeTier3(&limits);
        limits.v1.maxComputeWorkgroupStorageSize =
            limitsComputeWorkgroupStorageSizeTier1.v1.maxComputeWorkgroupStorageSize + 1;
        limits.v1.maxStorageBufferBindingSize =
            limitsStorageBufferBindingSizeTier2.v1.maxStorageBufferBindingSize + 1;

        CombinedLimits tiered = ApplyLimitTiers(limits);

        CombinedLimits expected = tiered;
        SetLimitsComputeWorkgroupStorageSizeTier1(&expected);
        SetLimitsStorageBufferBindingSizeTier2(&expected);
        EXPECT_EQ(tiered.v1, expected.v1);
    }
}

// Test that |ApplyLimitTiers| will hold the maxStorageBufferBindingSize no larger than
// maxBufferSize restriction.
TEST(Limits, TieredMaxStorageBufferBindingSizeNoLargerThanMaxBufferSize) {
    // Start with the default for supported.
    CombinedLimits defaults;
    GetDefaultLimits(&defaults, wgpu::FeatureLevel::Core);

    // Test reported maxStorageBufferBindingSize around 128MB, 1GB, 2GB-4 and 4GB-4.
    constexpr uint64_t storageSizeTier1 = 134217728ull;   // 128MB
    constexpr uint64_t storageSizeTier2 = 1073741824ull;  // 1GB
    constexpr uint64_t storageSizeTier3 = 2147483644ull;  // 2GB-4
    constexpr uint64_t storageSizeTier4 = 4294967292ull;  // 4GB-4
    constexpr uint64_t possibleReportedMaxStorageBufferBindingSizes[] = {
        storageSizeTier1,     storageSizeTier1 + 1, storageSizeTier2 - 1, storageSizeTier2,
        storageSizeTier2 + 1, storageSizeTier3 - 1, storageSizeTier3,     storageSizeTier3 + 1,
        storageSizeTier4 - 1, storageSizeTier4,     storageSizeTier4 + 1};
    // Test reported maxBufferSize around 256MB, 1GB, 2GB and 4GB, and a large 256GB.
    constexpr uint64_t bufferSizeTier1 = 0x10000000ull;    // 256MB
    constexpr uint64_t bufferSizeTier2 = 0x40000000ull;    // 1GB
    constexpr uint64_t bufferSizeTier3 = 0x80000000ull;    // 2GB
    constexpr uint64_t bufferSizeTier4 = 0x100000000ull;   // 4GB
    constexpr uint64_t bufferSizeLarge = 0x4000000000ull;  // 256GB
    constexpr uint64_t possibleReportedMaxBufferSizes[] = {
        bufferSizeTier1,     bufferSizeTier1 + 1, bufferSizeTier2 - 1, bufferSizeTier2,
        bufferSizeTier2 + 1, bufferSizeTier3 - 1, bufferSizeTier3,     bufferSizeTier3 + 1,
        bufferSizeTier4 - 1, bufferSizeTier4,     bufferSizeTier4 + 1, bufferSizeLarge};

    // Test that tiered maxStorageBufferBindingSize is no larger than tiered maxBufferSize.
    for (uint64_t reportedMaxStorageBufferBindingSizes :
         possibleReportedMaxStorageBufferBindingSizes) {
        for (uint64_t reportedMaxBufferSizes : possibleReportedMaxBufferSizes) {
            CombinedLimits limits = defaults;
            limits.v1.maxStorageBufferBindingSize = reportedMaxStorageBufferBindingSizes;
            limits.v1.maxBufferSize = reportedMaxBufferSizes;

            CombinedLimits tiered = ApplyLimitTiers(limits);

            EXPECT_LE(tiered.v1.maxStorageBufferBindingSize, tiered.v1.maxBufferSize);
        }
    }
}

// Test that |ApplyLimitTiers| will hold the maxUniformBufferBindingSize no larger than
// maxBufferSize restriction.
TEST(Limits, TieredMaxUniformBufferBindingSizeNoLargerThanMaxBufferSize) {
    // Start with the default for supported.
    CombinedLimits defaults;
    GetDefaultLimits(&defaults, wgpu::FeatureLevel::Core);

    // Test reported maxStorageBufferBindingSize around 64KB, and a large 1GB.
    constexpr uint64_t uniformSizeTier1 = 65536ull;       // 64KB
    constexpr uint64_t uniformSizeLarge = 1073741824ull;  // 1GB
    constexpr uint64_t possibleReportedMaxUniformBufferBindingSizes[] = {
        uniformSizeTier1, uniformSizeTier1 + 1, uniformSizeLarge};
    // Test reported maxBufferSize around 256MB, 1GB, 2GB and 4GB, and a large 256GB.
    constexpr uint64_t bufferSizeTier1 = 0x10000000ull;    // 256MB
    constexpr uint64_t bufferSizeTier2 = 0x40000000ull;    // 1GB
    constexpr uint64_t bufferSizeTier3 = 0x80000000ull;    // 2GB
    constexpr uint64_t bufferSizeTier4 = 0x100000000ull;   // 4GB
    constexpr uint64_t bufferSizeLarge = 0x4000000000ull;  // 256GB
    constexpr uint64_t possibleReportedMaxBufferSizes[] = {
        bufferSizeTier1,     bufferSizeTier1 + 1, bufferSizeTier2 - 1, bufferSizeTier2,
        bufferSizeTier2 + 1, bufferSizeTier3 - 1, bufferSizeTier3,     bufferSizeTier3 + 1,
        bufferSizeTier4 - 1, bufferSizeTier4,     bufferSizeTier4 + 1, bufferSizeLarge};

    // Test that tiered maxUniformBufferBindingSize is no larger than tiered maxBufferSize.
    for (uint64_t reportedMaxUniformBufferBindingSizes :
         possibleReportedMaxUniformBufferBindingSizes) {
        for (uint64_t reportedMaxBufferSizes : possibleReportedMaxBufferSizes) {
            CombinedLimits limits = defaults;
            limits.v1.maxUniformBufferBindingSize = reportedMaxUniformBufferBindingSizes;
            limits.v1.maxBufferSize = reportedMaxBufferSizes;

            CombinedLimits tiered = ApplyLimitTiers(limits);

            EXPECT_LE(tiered.v1.maxUniformBufferBindingSize, tiered.v1.maxBufferSize);
        }
    }
}

// Test |NormalizeLimits| works to enforce restriction of limits.
TEST(Limits, NormalizeLimits) {
    // Start with the default for supported.
    CombinedLimits defaults;
    GetDefaultLimits(&defaults, wgpu::FeatureLevel::Core);

    // Test specific limit values are clamped to internal Dawn constants.
    {
        CombinedLimits limits = defaults;
        limits.v1.maxVertexBufferArrayStride = kMaxVertexBufferArrayStride + 1;
        limits.v1.maxColorAttachments = uint32_t(kMaxColorAttachments) + 1;
        limits.v1.maxBindGroups = kMaxBindGroups + 1;
        limits.v1.maxBindGroupsPlusVertexBuffers = kMaxBindGroupsPlusVertexBuffers + 1;
        limits.v1.maxVertexAttributes = uint32_t(kMaxVertexAttributes) + 1;
        limits.v1.maxVertexBuffers = uint32_t(kMaxVertexBuffers) + 1;
        limits.v1.maxSampledTexturesPerShaderStage = kMaxSampledTexturesPerShaderStage + 1;
        limits.v1.maxSamplersPerShaderStage = kMaxSamplersPerShaderStage + 1;
        limits.v1.maxStorageBuffersPerShaderStage = kMaxStorageBuffersPerShaderStage + 1;
        limits.v1.maxStorageTexturesPerShaderStage = kMaxStorageTexturesPerShaderStage + 1;
        limits.v1.maxUniformBuffersPerShaderStage = kMaxUniformBuffersPerShaderStage + 1;
        limits.v1.maxImmediateSize = kMaxSupportedImmediateDataBytes + 1;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxVertexBufferArrayStride, kMaxVertexBufferArrayStride);
        EXPECT_EQ(limits.v1.maxColorAttachments, uint32_t(kMaxColorAttachments));
        EXPECT_EQ(limits.v1.maxBindGroups, kMaxBindGroups);
        EXPECT_EQ(limits.v1.maxBindGroupsPlusVertexBuffers, kMaxBindGroupsPlusVertexBuffers);
        EXPECT_EQ(limits.v1.maxVertexAttributes, uint32_t(kMaxVertexAttributes));
        EXPECT_EQ(limits.v1.maxVertexBuffers, uint32_t(kMaxVertexBuffers));
        EXPECT_EQ(limits.v1.maxSampledTexturesPerShaderStage, kMaxSampledTexturesPerShaderStage);
        EXPECT_EQ(limits.v1.maxSamplersPerShaderStage, kMaxSamplersPerShaderStage);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, kMaxStorageBuffersPerShaderStage);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, kMaxStorageTexturesPerShaderStage);
        EXPECT_EQ(limits.v1.maxUniformBuffersPerShaderStage, kMaxUniformBuffersPerShaderStage);
        EXPECT_EQ(limits.v1.maxImmediateSize, kMaxSupportedImmediateDataBytes);
    }

    // Test maxStorageBufferBindingSize is clamped to maxBufferSize.
    // maxStorageBufferBindingSize is no larger than maxBufferSize
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxStorageBufferBindingSize = reportedMaxBufferSize;
        CombinedLimits limits = defaults;
        limits.v1.maxStorageBufferBindingSize = reportedMaxStorageBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxStorageBufferBindingSize, reportedMaxStorageBufferBindingSize);
    }
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxStorageBufferBindingSize = reportedMaxBufferSize - 1;
        CombinedLimits limits = defaults;
        limits.v1.maxStorageBufferBindingSize = reportedMaxStorageBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxStorageBufferBindingSize, reportedMaxStorageBufferBindingSize);
    }
    // maxStorageBufferBindingSize is equal to maxBufferSize+1, expect clamping to maxBufferSize
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxStorageBufferBindingSize = reportedMaxBufferSize + 1;
        CombinedLimits limits = defaults;
        limits.v1.maxStorageBufferBindingSize = reportedMaxStorageBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxStorageBufferBindingSize, reportedMaxBufferSize);
    }
    // maxStorageBufferBindingSize is much larger than maxBufferSize, expect clamping to
    // maxBufferSize
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxStorageBufferBindingSize = 4294967295;
        CombinedLimits limits = defaults;
        limits.v1.maxStorageBufferBindingSize = reportedMaxStorageBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxStorageBufferBindingSize, reportedMaxBufferSize);
    }

    // Test maxUniformBufferBindingSize is clamped to maxBufferSize.
    // maxUniformBufferBindingSize is no larger than maxBufferSize
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxUniformBufferBindingSize = reportedMaxBufferSize - 1;
        CombinedLimits limits = defaults;
        limits.v1.maxUniformBufferBindingSize = reportedMaxUniformBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxUniformBufferBindingSize, reportedMaxUniformBufferBindingSize);
    }
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxUniformBufferBindingSize = reportedMaxBufferSize;
        CombinedLimits limits = defaults;
        limits.v1.maxUniformBufferBindingSize = reportedMaxUniformBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxUniformBufferBindingSize, reportedMaxUniformBufferBindingSize);
    }
    // maxUniformBufferBindingSize is larger than maxBufferSize, expect clamping to maxBufferSize
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxUniformBufferBindingSize = reportedMaxBufferSize + 1;
        CombinedLimits limits = defaults;
        limits.v1.maxUniformBufferBindingSize = reportedMaxUniformBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxUniformBufferBindingSize, reportedMaxBufferSize);
    }
    // maxUniformBufferBindingSize is much larger than maxBufferSize, expect clamping to
    // maxBufferSize
    {
        constexpr uint64_t reportedMaxBufferSize = 2147483648;
        constexpr uint64_t reportedMaxUniformBufferBindingSize = 4294967295;
        CombinedLimits limits = defaults;
        limits.v1.maxUniformBufferBindingSize = reportedMaxUniformBufferBindingSize;
        limits.v1.maxBufferSize = reportedMaxBufferSize;

        NormalizeLimits(&limits);

        EXPECT_EQ(limits.v1.maxBufferSize, reportedMaxBufferSize);
        EXPECT_EQ(limits.v1.maxUniformBufferBindingSize, reportedMaxBufferSize);
    }
}

// Test |FixupDeviceLimits| works fix up the limits
TEST(Limits, FixupDeviceLimits) {
    // Test maxXXXInStage is raised to maxXXXPerStage in core
    {
        CombinedLimits limits;
        limits.compat.maxStorageBuffersInFragmentStage = 1;
        limits.compat.maxStorageBuffersInVertexStage = 2;
        limits.v1.maxStorageBuffersPerShaderStage = 3;

        limits.compat.maxStorageTexturesInFragmentStage = 4;
        limits.compat.maxStorageTexturesInVertexStage = 5;
        limits.v1.maxStorageTexturesPerShaderStage = 6;

        EnforceLimitSpecInvariants(&limits, wgpu::FeatureLevel::Core);

        EXPECT_EQ(limits.compat.maxStorageBuffersInFragmentStage, 3u);
        EXPECT_EQ(limits.compat.maxStorageBuffersInVertexStage, 3u);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, 3u);

        EXPECT_EQ(limits.compat.maxStorageTexturesInFragmentStage, 6u);
        EXPECT_EQ(limits.compat.maxStorageTexturesInVertexStage, 6u);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, 6u);
    }

    // Test maxXXXInStage are not raised to maxXXXPerStage in compat
    {
        CombinedLimits limits;
        limits.compat.maxStorageBuffersInFragmentStage = 1;
        limits.compat.maxStorageBuffersInVertexStage = 2;
        limits.v1.maxStorageBuffersPerShaderStage = 3;

        limits.compat.maxStorageTexturesInFragmentStage = 4;
        limits.compat.maxStorageTexturesInVertexStage = 5;
        limits.v1.maxStorageTexturesPerShaderStage = 6;

        EnforceLimitSpecInvariants(&limits, wgpu::FeatureLevel::Compatibility);

        EXPECT_EQ(limits.compat.maxStorageBuffersInFragmentStage, 1u);
        EXPECT_EQ(limits.compat.maxStorageBuffersInVertexStage, 2u);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, 3u);

        EXPECT_EQ(limits.compat.maxStorageTexturesInFragmentStage, 4u);
        EXPECT_EQ(limits.compat.maxStorageTexturesInVertexStage, 5u);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, 6u);
    }

    // Test maxXXXPerStage is raised to maxXXXInStage in core
    {
        CombinedLimits limits;
        limits.compat.maxStorageBuffersInFragmentStage = 3;
        limits.compat.maxStorageBuffersInVertexStage = 2;
        limits.v1.maxStorageBuffersPerShaderStage = 1;

        limits.compat.maxStorageTexturesInFragmentStage = 6;
        limits.compat.maxStorageTexturesInVertexStage = 5;
        limits.v1.maxStorageTexturesPerShaderStage = 4;

        EnforceLimitSpecInvariants(&limits, wgpu::FeatureLevel::Core);

        EXPECT_EQ(limits.compat.maxStorageBuffersInFragmentStage, 3u);
        EXPECT_EQ(limits.compat.maxStorageBuffersInVertexStage, 3u);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, 3u);

        EXPECT_EQ(limits.compat.maxStorageTexturesInFragmentStage, 6u);
        EXPECT_EQ(limits.compat.maxStorageTexturesInVertexStage, 6u);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, 6u);

        limits.compat.maxStorageBuffersInFragmentStage = 2;
        limits.compat.maxStorageBuffersInVertexStage = 3;
        limits.v1.maxStorageBuffersPerShaderStage = 1;

        limits.compat.maxStorageTexturesInFragmentStage = 5;
        limits.compat.maxStorageTexturesInVertexStage = 6;
        limits.v1.maxStorageTexturesPerShaderStage = 4;

        EnforceLimitSpecInvariants(&limits, wgpu::FeatureLevel::Core);

        EXPECT_EQ(limits.compat.maxStorageBuffersInFragmentStage, 3u);
        EXPECT_EQ(limits.compat.maxStorageBuffersInVertexStage, 3u);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, 3u);

        EXPECT_EQ(limits.compat.maxStorageTexturesInFragmentStage, 6u);
        EXPECT_EQ(limits.compat.maxStorageTexturesInVertexStage, 6u);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, 6u);
    }

    // Test maxXXXPerStage is raised to maxXXXInStage compat
    {
        CombinedLimits limits;
        limits.compat.maxStorageBuffersInFragmentStage = 3;
        limits.compat.maxStorageBuffersInVertexStage = 2;
        limits.v1.maxStorageBuffersPerShaderStage = 1;

        limits.compat.maxStorageTexturesInFragmentStage = 6;
        limits.compat.maxStorageTexturesInVertexStage = 5;
        limits.v1.maxStorageTexturesPerShaderStage = 4;

        EnforceLimitSpecInvariants(&limits, wgpu::FeatureLevel::Compatibility);

        EXPECT_EQ(limits.compat.maxStorageBuffersInFragmentStage, 3u);
        EXPECT_EQ(limits.compat.maxStorageBuffersInVertexStage, 2u);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, 3u);

        EXPECT_EQ(limits.compat.maxStorageTexturesInFragmentStage, 6u);
        EXPECT_EQ(limits.compat.maxStorageTexturesInVertexStage, 5u);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, 6u);

        limits.compat.maxStorageBuffersInFragmentStage = 2;
        limits.compat.maxStorageBuffersInVertexStage = 3;
        limits.v1.maxStorageBuffersPerShaderStage = 1;

        limits.compat.maxStorageTexturesInFragmentStage = 5;
        limits.compat.maxStorageTexturesInVertexStage = 6;
        limits.v1.maxStorageTexturesPerShaderStage = 4;

        EnforceLimitSpecInvariants(&limits, wgpu::FeatureLevel::Compatibility);

        EXPECT_EQ(limits.compat.maxStorageBuffersInFragmentStage, 2u);
        EXPECT_EQ(limits.compat.maxStorageBuffersInVertexStage, 3u);
        EXPECT_EQ(limits.v1.maxStorageBuffersPerShaderStage, 3u);

        EXPECT_EQ(limits.compat.maxStorageTexturesInFragmentStage, 5u);
        EXPECT_EQ(limits.compat.maxStorageTexturesInVertexStage, 6u);
        EXPECT_EQ(limits.v1.maxStorageTexturesPerShaderStage, 6u);
    }
}

}  // namespace native
}  // namespace dawn
