/*==================================================================*\
  BatchCoordinator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/BatchCoordinator.hpp>
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	VkResult BatchCoordinator::BindResources( GpuHeap& heap ) {
		ET_FAIL_UNLESS( _drawParameters.BindResources( heap, ParameterBufferSize ) );

		return VK_SUCCESS;
	}

//---------------------------------------------------

	void BatchCoordinator::FreeResources( GpuHeap& heap ) {
		_drawParameters.FreeResources( heap );
	}

//---------------------------------------------------

	void BatchCoordinator::AddThing( void* thing ) {
		_countsByThing.TryEmplace( thing, 0u );
	}

//---------------------------------------------------

	void BatchCoordinator::ResetCounts() {
		for (ArrayMap<void*, uint32_t>::ValueType& value : _countsByThing) {
			value.second = 0u;
		}
	}

//---------------------------------------------------

	void BatchCoordinator::RecordDraws( CommandList& commands, VertexBuffer& vertices ) {
		commands.BindBuffers( { vertices.Get() }, static_cast<VkBuffer>(VK_NULL_HANDLE), VK_INDEX_TYPE_UINT16 );
		commands.DrawIndexedIndirect( _drawParameters, 0u, _countsByThing.GetSize() );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2