/*==================================================================*\
  VulkanTools.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	template <typename VulkanType>
	ETConstexpr ETForceInlineHint ETPureFunctionHint NullHandle::operator VulkanType*() const ETNoexceptHint {
		return static_cast<VulkanType*>(VK_NULL_HANDLE);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool Succeeded(VkResult result) ETNoexceptHint {
		return result == VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETConstexpr ETPureFunctionHint bool Failed(VkResult result) ETNoexceptHint {
		return result < VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint FormatDescriptor AsVulkan(GpuFormat format) ETNoexceptHint {
		switch (format) {
		case GpuFormat::R8G8B8A8_Unorm: return { VK_FORMAT_R8G8B8A8_UNORM, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::R8G8B8A8_Srgb: return { VK_FORMAT_R8G8B8A8_SRGB, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::R8G8B8A8_Snorm: return { VK_FORMAT_R8G8B8A8_SNORM, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::R8G8B8A8_Uint: return { VK_FORMAT_R8G8B8A8_UINT, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::BC1_Unorm: return { VK_FORMAT_BC1_RGB_UNORM_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC1_Srgb: return { VK_FORMAT_BC1_RGB_SRGB_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC3_Unorm: return { VK_FORMAT_BC3_UNORM_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::BC3_Srgb: return { VK_FORMAT_BC3_SRGB_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::BC3_YCoCg: return { VK_FORMAT_BC3_UNORM_BLOCK, { VK_COMPONENT_SWIZZLE_A, VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC5_Unorm: return { VK_FORMAT_BC5_UNORM_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC5_Snorm: return { VK_FORMAT_BC5_SNORM_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC6_Float: return { VK_FORMAT_BC6H_SFLOAT_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC7_Unorm: return { VK_FORMAT_BC7_UNORM_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::BC7_Srgb: return { VK_FORMAT_BC7_SRGB_BLOCK, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ONE } };
		case GpuFormat::R16G16B16A16_Float: return { VK_FORMAT_R16G16B16A16_SFLOAT, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::R32G32B32A32_Float: return { VK_FORMAT_R32G32B32A32_SFLOAT, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::D32_Float: return { VK_FORMAT_D32_SFLOAT, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::D32_Float_S8_Uint: return { VK_FORMAT_D32_SFLOAT_S8_UINT, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::D16_Float: return { VK_FORMAT_D16_UNORM, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		case GpuFormat::D16_Float_S8_Uint: return { VK_FORMAT_D16_UNORM_S8_UINT, { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A } };
		default: return { VK_FORMAT_UNDEFINED, { VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO, VK_COMPONENT_SWIZZLE_ZERO } };
		};
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkFormat AsVkFormat(GpuFormat format) ETNoexceptHint {
		return AsVulkan(format).format;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent) ETNoexceptHint {
		return extent.depth > 1u ? VK_IMAGE_TYPE_3D : (extent.height > 1u ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_1D);
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint VkImageViewType GetImageType(VkImageCreateFlags flags, VkImageType type, uint32 layers) ETNoexceptHint {
#pragma push_macro("ET_MAKE_SWITCH_CASE")
#define ET_MAKE_SWITCH_CASE(imageType, isCube, isArray) int(int(imageType) | (bool(isCube) << 4) | (bool(isArray) << 3))
		switch (ET_MAKE_SWITCH_CASE(type, flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT, layers > (flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ? 6u : 1u))) {
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_1D, false, false): return VK_IMAGE_VIEW_TYPE_1D; //clang-format off
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_2D, false, false): return VK_IMAGE_VIEW_TYPE_2D;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_3D, false, false): return VK_IMAGE_VIEW_TYPE_3D;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_1D, false, true): return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_2D, false, true): return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_3D, false, true): return VK_IMAGE_VIEW_TYPE_3D;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_1D, true, false): return VK_IMAGE_VIEW_TYPE_1D;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_2D, true, false): return VK_IMAGE_VIEW_TYPE_CUBE;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_3D, true, false): return VK_IMAGE_VIEW_TYPE_3D;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_1D, true, true): return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_2D, true, true): return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		case ET_MAKE_SWITCH_CASE(VK_IMAGE_TYPE_3D, true, true): return VK_IMAGE_VIEW_TYPE_3D;
		default: return VK_IMAGE_VIEW_TYPE_2D; // clang-format on
		};
#pragma pop_macro("ET_MAKE_SWITCH_CASE")
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool reverseDepth) ETNoexceptHint {
		return VkViewport{
			/*x =*/float(renderArea.offset.x),
			/*y =*/float(renderArea.offset.y),
			/*width =*/float(renderArea.extent.width),
			/*height =*/float(renderArea.extent.height),
			/*minDepth =*/reverseDepth ? 1.0f : 0.0f,
			/*maxDepth =*/reverseDepth ? 0.0f : 1.0f
		};
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkSampleCountFlagBits GetSampleCountFlags(uint32 samples) ETNoexceptHint {
		samples = Clamp<uint32_t>(samples, VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_64_BIT);
		return VkSampleCountFlagBits(samples | (samples - 1u));
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkImageAspectFlags GetAspects(VkImageUsageFlags usages) ETNoexceptHint {
		ETConstexpr VkImageUsageFlags ColorUsages(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		ETConstexpr VkImageUsageFlags DepthUsages(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		return (usages & ColorUsages ? VK_IMAGE_ASPECT_COLOR_BIT : 0u) | (usages & DepthUsages ? (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : 0u);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint size_t GetHashCode(VkFormat format, size_t seed) ETNoexceptHint {
		return size_t(format) * 31u + seed;
	}

}}} // namespace Eldritch2::Graphics::Vulkan
