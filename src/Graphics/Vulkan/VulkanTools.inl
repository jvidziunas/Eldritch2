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

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint bool Succeeded(VkResult result) ETNoexceptHint {
		return result == VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETPureFunctionHint bool Failed(VkResult result) ETNoexceptHint {
		return result < VK_SUCCESS;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkImageType GetImageType(VkExtent3D extent) ETNoexceptHint {
		return extent.depth > 1u ? VK_IMAGE_TYPE_3D : (extent.height > 1u ? VK_IMAGE_TYPE_2D : VK_IMAGE_TYPE_1D);
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint VkImageViewType GetViewType(VkImageCreateFlags flags, VkImageType type, uint32 layers) ETNoexceptHint {
		ETConstexpr VkImageViewType types[VK_IMAGE_TYPE_RANGE_SIZE][2] = {
			{ VK_IMAGE_VIEW_TYPE_1D, VK_IMAGE_VIEW_TYPE_1D_ARRAY },
			{ VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_VIEW_TYPE_2D_ARRAY },
			{ VK_IMAGE_VIEW_TYPE_3D, VK_IMAGE_VIEW_TYPE_2D_ARRAY },
		};

		return (flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT) ? (layers > 6u ? VK_IMAGE_VIEW_TYPE_CUBE_ARRAY : VK_IMAGE_VIEW_TYPE_CUBE) : types[type][layers > 1u];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint VkViewport GetViewport(VkRect2D renderArea, bool invertDepth) ETNoexceptHint {
		return VkViewport {
			/*x =*/AsFloat(renderArea.offset.x),
			/*y =*/AsFloat(renderArea.offset.y),
			/*width =*/AsFloat(renderArea.extent.width),
			/*height =*/AsFloat(renderArea.extent.height),
			/*minDepth =*/invertDepth ? 1.0f : 0.0f,
			/*maxDepth =*/invertDepth ? 0.0f : 1.0f
		};
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkSampleCountFlagBits GetSampleCountFlags(uint32 samples) ETNoexceptHint {
		samples = Clamp<uint32_t>(samples, VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_64_BIT);
		return VkSampleCountFlagBits(samples | (samples - 1u));
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETPureFunctionHint VkImageAspectFlags GetAspectsByUsage(VkImageUsageFlags usages) ETNoexceptHint {
		ETConstexpr VkImageUsageFlags ColorUsages(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
		ETConstexpr VkImageUsageFlags DepthUsages(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);

		return (usages & ColorUsages ? VK_IMAGE_ASPECT_COLOR_BIT : 0u) | (usages & DepthUsages ? (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT) : 0u);
	}

}}} // namespace Eldritch2::Graphics::Vulkan
