/*==================================================================*\
  GpuHeap.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanTools.hpp>
#include <Graphics/Vulkan/GpuHeap.hpp>
#include <Graphics/Vulkan/Gpu.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	GpuHeap::GpuHeap(
	) : _allocator( nullptr ),
		_imageGarbage( MallocAllocator( "Vulkan GPU Heap Image Garbage List Allocator" ) ),
		_bufferGarbage( MallocAllocator( "Vulkan GPU Heap Buffer Garbage List Allocator" ) ) {
	}

// ---------------------------------------------------

	GpuHeap::GpuHeap(
		GpuHeap&& allocator
	) : _allocator( eastl::exchange( allocator._allocator, nullptr ) ),
		_imageGarbage( eastl::move( allocator._imageGarbage ) ),
		_bufferGarbage( eastl::move( allocator._bufferGarbage ) ) {}

// ---------------------------------------------------

	GpuHeap::~GpuHeap() {
		ET_ASSERT( _allocator == nullptr, "Leaking Vulkan allocator instance!" );
	}

// ---------------------------------------------------

	void GpuHeap::DestroyGarbage() {
		GarbageList<VkImage>	imageGarbage( _imageGarbage.GetAllocator() );
		GarbageList<VkBuffer>	bufferGarbage( _bufferGarbage.GetAllocator() );

		{	Lock	_( _garbageMutex );
			Swap( _imageGarbage,	imageGarbage );
			Swap( _bufferGarbage,	bufferGarbage );
		}	// End of lock scope.

		for (const Pair<VkImage, VmaAllocation>& garbage : imageGarbage) {
			vmaDestroyImage( _allocator, garbage.first, garbage.second );
		}

		for (const Pair<VkBuffer, VmaAllocation>& garbage : bufferGarbage) {
			vmaDestroyBuffer( _allocator, garbage.first, garbage.second );
		}
	}

// ---------------------------------------------------

	VkResult GpuHeap::BindResources( Gpu& gpu, VkDeviceSize heapBlockSize ) {
		using ::Eldritch2::Swap;

		VmaAllocator allocator;
		const VmaAllocatorCreateInfo allocatorInfo{
			VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT,
			static_cast<VkPhysicalDevice>(gpu),
			static_cast<VkDevice>(gpu),
			heapBlockSize,
			gpu.GetAllocationCallbacks(),
			nullptr
		};

		ET_FAIL_UNLESS( vmaCreateAllocator( &allocatorInfo, &allocator ) );
		ET_AT_SCOPE_EXIT( vmaDestroyAllocator( allocator ) );

		Swap( _allocator, allocator );

		return VK_SUCCESS;
	}

// ---------------------------------------------------

	void GpuHeap::FreeResources( Gpu& /*gpu*/ ) {
		DestroyGarbage();

		vmaDestroyAllocator( eastl::exchange( _allocator, nullptr ) );
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2