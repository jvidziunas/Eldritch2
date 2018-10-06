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

	extern const struct FormatDescriptor {
		VkFormat           deviceFormat;
		VkComponentMapping shaderSwizzle;
	} TextureFormats[size_t(GpuFormat::MAX) + 1];

	ETCpp14Constexpr ETPureFunctionHint VkSampleCountFlagBits GetSampleCountFlags(uint32 samples) ETNoexceptHint;

	ETCpp14Constexpr ETPureFunctionHint VkImageAspectFlags GetAspectsByUsage(VkImageUsageFlags usages) ETNoexceptHint;

	ETCpp14Constexpr ETPureFunctionHint VkImageViewType GetViewType(VkImageCreateFlags flags, VkImageType type, uint32 layers) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent) ETNoexceptHint;

	// ---------------------------------------------------

	ETConstexpr ETPureFunctionHint bool Succeeded(VkResult result) ETNoexceptHint;

	ETConstexpr ETPureFunctionHint bool Failed(VkResult result) ETNoexceptHint;

	// ---------------------------------------------------

	ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool invertDepth = true) ETNoexceptHint;

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.inl>
//------------------------------------------------------------------//
