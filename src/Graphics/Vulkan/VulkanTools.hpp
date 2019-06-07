/*==================================================================*\
  VulkanTools.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GpuAbi.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	struct FormatDescriptor {
		VkFormat           format;
		VkComponentMapping swizzle;
	};

	// ---

	struct NullHandle {
		template <typename VulkanType>
		ETConstexpr ETPureFunctionHint operator VulkanType*() const ETNoexceptHint;
	};

	// ---

	ETConstexpr ETPureFunctionHint bool Succeeded(VkResult result) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint bool Failed(VkResult result) ETNoexceptHint;

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint FormatDescriptor AsVulkan(GpuFormat) ETNoexceptHint;
	ETCpp14Constexpr ETPureFunctionHint VkFormat AsVkFormat(GpuFormat) ETNoexceptHint;

	ETCpp14Constexpr ETPureFunctionHint VkSampleCountFlagBits GetSampleCountFlags(uint32 samples) ETNoexceptHint;

	ETCpp14Constexpr ETPureFunctionHint VkImageAspectFlags GetAspects(VkImageUsageFlags usages) ETNoexceptHint;

	ETCpp14Constexpr ETPureFunctionHint VkImageViewType GetImageType(VkImageCreateFlags flags, VkImageType type, uint32 layers) ETNoexceptHint;
	ETConstexpr ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent) ETNoexceptHint;

	// ---------------------------------------------------

	ETConstexpr ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool reverseDepth = true) ETNoexceptHint;

	// ---------------------------------------------------

	ETConstexpr ETPureFunctionHint size_t GetHashCode(VkFormat format, size_t seed = 0u) ETNoexceptHint;

	// ---------------------------------------------------

	ETConstexpr VkOffset2D ScreenOrigin{ 0, 0 };

	ETConstexpr NullHandle NullVulkanHandle;

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.inl>
//------------------------------------------------------------------//
