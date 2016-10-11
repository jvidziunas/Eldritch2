/*==================================================================*\
  Device.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/VulkanResult.hpp>
#include <Renderer/Vulkan/Device.hpp>
//------------------------------------------------------------------//

#if defined( CreateSemaphore )
#	pragma push_macro( "CreateSemaphore" )
#	undef CreateSemaphore
#	define CLEANUP_CREATESEMAPHORE 1
#endif	// defined( CreateSemaphore )
#if defined( CreateEvent )
#	pragma push_macro( "CreateEvent" )
#	undef CreateEvent
#	define CLEANUP_CREATEEVENT 1
#endif	// defined( CreateEvent )
namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace {

	enum : VkBool32 {
		WaitAll = VK_TRUE
	};

}	// anonymous namespace

	Device::Device( VkPhysicalDevice physicalDevice, UniquePointer<VkDevice> device, Allocator& allocator ) : _allocator( allocator, "Vulkan Device Context Allocator" ),
																											  _physicalDevice( eastl::move( physicalDevice ) ),
																											  _presentQueue( VK_NULL_HANDLE ),
																											  _sparseBindQueue( VK_NULL_HANDLE ),
																											  _resourceCopyQueue( VK_NULL_HANDLE ),
																											  _device( eastl::move( device ) ),
																											  _pipelineCache( nullptr ),
																											  _presentFence( eastl::move( *CreateFence( VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT ) ) ),
																											  _copyFence( eastl::move( *CreateFence( VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT ) ) ),
																											  _copySemaphore( eastl::move( *CreateSemaphore( 0 ) ) ),
																											  _copyCommandPool( eastl::move( *CreateCommandPool( VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_TRANSIENT_BIT, 0u ) ) ),
																											  _specializationConstants( { allocator, "Vulkan Device Context Specialization Constant Bucket Allocator" } ) {
	}

// ---------------------------------------------------

	VulkanResult<VkDeviceMemory> Device::AllocateMemory( VkDeviceSize allocationSizeInBytes, uint32 memoryTypeIndex ) {
		VkMemoryAllocateInfo	allocateInfo;
		VkDeviceMemory			deviceMemory( VK_NULL_HANDLE );

		allocateInfo.sType				= VkStructureType::VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.pNext				= nullptr;
		allocateInfo.allocationSize		= allocationSizeInBytes;
		allocateInfo.memoryTypeIndex	= memoryTypeIndex;
		
		const auto	result( vkAllocateMemory( _device.Get(), &allocateInfo, _allocator, &deviceMemory ) );
		if( result < VkResult::VK_SUCCESS ) {
			return { result };
		}

		return Vulkan::UniquePointer<VkDeviceMemory>( deviceMemory, { _device.Get(), _allocator } );
	}

// ---------------------------------------------------

	VulkanResult<VkCommandPool> Device::CreateCommandPool( VkCommandPoolCreateFlags flags, uint32 queueFamilyIndex ) {
		VkCommandPoolCreateInfo	createInfo;
		VkCommandPool			pool( VK_NULL_HANDLE );

		createInfo.sType			= VkStructureType::VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext			= nullptr;
		createInfo.flags			= flags;
		createInfo.queueFamilyIndex	= queueFamilyIndex;

		const auto	result( vkCreateCommandPool( _device.Get(), &createInfo, _allocator, &pool ) );
		if( result < VkResult::VK_SUCCESS ) {
			return { result };
		}

		return Vulkan::UniquePointer<VkCommandPool>( pool, { _device.Get(), _allocator } );
	}

// ---------------------------------------------------

	VulkanResult<VkSemaphore> Device::CreateSemaphore( VkSemaphoreCreateFlags flags ) {
		VkSemaphoreCreateInfo	createInfo;
		VkSemaphore				semaphore( VK_NULL_HANDLE );

		createInfo.sType	= VkStructureType::VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		createInfo.pNext	= nullptr;
		createInfo.flags	= flags;

		const auto result( vkCreateSemaphore( _device.Get(), &createInfo, _allocator, &semaphore ) );
		if( result < VkResult::VK_SUCCESS ) {
			return { result };
		}

		return Vulkan::UniquePointer<VkSemaphore>( semaphore, { _device.Get(), _allocator } );
	}

// ---------------------------------------------------

	VulkanResult<VkFence> Device::CreateFence( VkFenceCreateFlags flags ) {
		VkFenceCreateInfo	createInfo;
		VkFence				fence( VK_NULL_HANDLE );

		createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = flags;

		const auto result( vkCreateFence( _device.Get(), &createInfo, _allocator, &fence ) );
		if( result < VkResult::VK_SUCCESS ) {
			return { result };
		}

		return Vulkan::UniquePointer<VkFence>( fence, { _device.Get(), _allocator } );
	}

// ---------------------------------------------------

	VulkanResult<VkEvent> Device::CreateEvent( VkEventCreateFlags flags ) {
		VkEventCreateInfo	createInfo;
		VkEvent				event( VK_NULL_HANDLE );

		createInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = flags;

		const auto result( vkCreateEvent( _device.Get(), &createInfo, _allocator, &event ) );
		if( result < VkResult::VK_SUCCESS ) {
			return{ result };
		}

		return Vulkan::UniquePointer<VkEvent>( event, { _device.Get(), _allocator } );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2
#if defined( CLEANUP_CREATEEVENT )
#	pragma pop_macro( "CreateEvent" )
#	undef CLEANUP_CREATEEVENT
#endif	// defined( CLEANUP_CREATEEVENT )
#if defined( CLEANUP_CREATESEMAPHORE )
#	pragma pop_macro( "CreateSemaphore" )
#	undef CLEANUP_CREATESEMAPHORE
#endif	// defined( CLEANUP_CREATESEMAPHORE )
