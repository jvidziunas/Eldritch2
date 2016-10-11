/*==================================================================*\
  DeviceMemoryPool.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/DeviceMemoryPool.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	DeviceMemoryPool::DeviceMemoryPool( const DeviceMemoryPool& ) {
	//	This method is provided for signature compatibility with standard container types and is not intended for use in practice.
		ETRuntimeAssert( false );
	}

// ---------------------------------------------------

	DeviceMemoryPool::~DeviceMemoryPool() {
	//	TODO: Check if there are any regions still in use; warn the user if this is the case.
	}

// ---------------------------------------------------

	Result<DeviceMemoryPool::Allocation> DeviceMemoryPool::TryAllocateRegion( VkDeviceSize regionSizeInBytes ) {
		const auto	allocateResult( _availableRegions.AllocateRangeOfSize( regionSizeInBytes ) );

		if( !allocateResult.second ) {
			return Error::OutOfMemory;
		}

		return Allocation( { allocateResult.first, regionSizeInBytes }, *this );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

