/*==================================================================*\
  HostAllocator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/HostAllocator.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	HostAllocator::HostAllocator( Allocator& backingAllocator, const UTF8Char* const name ) : ChildAllocator( backingAllocator, name ) {
		_allocationCallbacks.pUserData				= this;
		_allocationCallbacks.pfnAllocation			= [] ( void* allocator, size_t sizeInBytes, size_t alignmentInBytes, VkSystemAllocationScope /*allocationScope*/ ) -> void* {
			return static_cast<HostAllocator*>(allocator)->Allocate( sizeInBytes, alignmentInBytes, Allocator::AllocationDuration::Normal );
		};
		_allocationCallbacks.pfnReallocation		= [] ( void* allocator, void* originalMemory, size_t sizeInBytes, size_t alignmentInBytes, VkSystemAllocationScope /*allocationScope*/ ) -> void* {
			return static_cast<HostAllocator*>(allocator)->Reallocate( originalMemory, sizeInBytes, alignmentInBytes, Allocator::AllocationDuration::Normal );
		};
		_allocationCallbacks.pfnFree				= [] ( void* allocator, void* memory ) {
			static_cast<HostAllocator*>(allocator)->Deallocate( memory, AlignedDeallocationSemantics );
		};
		_allocationCallbacks.pfnInternalAllocation	= [] ( void* /*allocator*/, size_t /*sizeInBytes*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/ ) {};
		_allocationCallbacks.pfnInternalFree		= [] ( void* /*allocator*/, size_t /*sizeInBytes*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/ ) {};
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



