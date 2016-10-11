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
#include <Utility/Memory/StandardLibrary.hpp>
#include <Renderer/Vulkan/HostAllocator.hpp>
//------------------------------------------------------------------//
#include <EASTL/utility.h>
//------------------------------------------------------------------//

#ifdef CopyMemory
#	undef CopyMemory
#endif

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace {

	static ETPureFunctionHint ETRestrictHint void* VKAPI_CALL VulkanAllocate( void* allocator, size_t sizeInBytes, size_t alignmentInBytes, VkSystemAllocationScope /*allocationScope*/ ) {
		size_t*	result( static_cast<size_t*>(static_cast<HostAllocator*>(allocator)->Allocate( sizeInBytes + sizeof( size_t ), alignmentInBytes, sizeof( size_t ), AllocationDuration::Normal )) );

		if( result ) {
			*result = sizeInBytes + sizeof(size_t);
		}

		return result + 1;
	}

// ---------------------------------------------------

	static ETPureFunctionHint void VKAPI_CALL VulkanFree( void* allocator, void* memory ) {
		if( nullptr == memory ) {
			return;
		}

		size_t* const	realAllocation( static_cast<size_t*>(memory)-1 );

		static_cast<HostAllocator*>(allocator)->Deallocate( realAllocation, *realAllocation );
	}

// ---------------------------------------------------

	static ETPureFunctionHint ETRestrictHint void* VKAPI_CALL VulkanReallocate( void* allocator, void* originalMemory, size_t sizeInBytes, size_t alignmentInBytes, VkSystemAllocationScope allocationScope ) {
		auto	result( VulkanAllocate( allocator, sizeInBytes, alignmentInBytes, allocationScope ) );

		if( result && originalMemory ) {
			CopyMemory( result, originalMemory, static_cast<size_t*>( originalMemory )[-1] );
		}

		VulkanFree( allocator, originalMemory );

		return result;
	}

}	// anonymous namespace

	HostAllocator::HostAllocator( Allocator& backingAllocator, const Utf8Char* const name ) : ChildAllocator( backingAllocator, name ) {
		_allocationCallbacks.pUserData				= this;
		_allocationCallbacks.pfnAllocation			= &VulkanAllocate; 
		_allocationCallbacks.pfnReallocation		= &VulkanReallocate;
		_allocationCallbacks.pfnFree				= &VulkanFree;
		_allocationCallbacks.pfnInternalAllocation	= [] ( void* /*allocator*/, size_t /*sizeInBytes*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/ ) {};
		_allocationCallbacks.pfnInternalFree		= [] ( void* /*allocator*/, size_t /*sizeInBytes*/, VkInternalAllocationType /*type*/, VkSystemAllocationScope /*scope*/ ) {};
	}

// ---------------------------------------------------

	HostAllocator::HostAllocator( HostAllocator&& allocator ) : ChildAllocator( eastl::forward<ChildAllocator>( allocator ) ), _allocationCallbacks( eastl::move( allocator._allocationCallbacks ) ) {}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2



