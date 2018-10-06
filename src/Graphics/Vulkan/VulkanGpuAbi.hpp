/*==================================================================*\
  VulkanGpuAbi.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
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

	ETConstexpr uint32 MeshSlot(0u);

	ETConstexpr VkVertexInputAttributeDescription SkinnedVertexAttributes[] = {
		{ /*location =*/MeshSlot, /*binding =*/0u, /*format =*/VK_FORMAT_R32G32B32A32_SFLOAT, /*offset =*/ETOffsetOf(SkinnedVertex, position) },
		{ /*location =*/MeshSlot, /*binding =*/1u, /*format =*/VK_FORMAT_R16G16B16A16_SNORM, /*offset =*/ETOffsetOf(SkinnedVertex, objectToTangent) },
		{ /*location =*/MeshSlot, /*binding =*/2u, /*format =*/VK_FORMAT_R16G16_UNORM, /*offset =*/ETOffsetOf(SkinnedVertex, texcoords[0]) },
		{ /*location =*/MeshSlot, /*binding =*/3u, /*format =*/VK_FORMAT_R16G16_UNORM, /*offset =*/ETOffsetOf(SkinnedVertex, texcoords[1]) },
		{ /*location =*/MeshSlot, /*binding =*/4u, /*format =*/VK_FORMAT_R8G8B8A8_UINT, /*offset =*/ETOffsetOf(SkinnedVertex, boneIndices[0]) },
		{ /*location =*/MeshSlot, /*binding =*/5u, /*format =*/VK_FORMAT_R8G8B8A8_UINT, /*offset =*/ETOffsetOf(SkinnedVertex, boneIndices[1]) },
		{ /*location =*/MeshSlot, /*binding =*/6u, /*format =*/VK_FORMAT_R8G8B8A8_UNORM, /*offset =*/ETOffsetOf(SkinnedVertex, boneWeights[1]) },
		{ /*location =*/MeshSlot, /*binding =*/7u, /*format =*/VK_FORMAT_R8G8B8A8_UNORM, /*offset =*/ETOffsetOf(SkinnedVertex, boneWeights[1]) },
	};

	// ---

	ETConstexpr VkVertexInputAttributeDescription UiVertexAttributes[] = {
		{ /*location =*/MeshSlot, /*binding =*/0u, /*format =*/VK_FORMAT_R16G16B16A16_UNORM, /*offset =*/ETOffsetOf(UiVertex, position) },
		{ /*location =*/MeshSlot, /*binding =*/1u, /*format =*/VK_FORMAT_R16G16_UNORM, /*offset =*/ETOffsetOf(UiVertex, texcoords[0]) },
		{ /*location =*/MeshSlot, /*binding =*/2u, /*format =*/VK_FORMAT_R16G16_UNORM, /*offset =*/ETOffsetOf(UiVertex, texcoords[1]) },
	};

}}} // namespace Eldritch2::Graphics::Vulkan
