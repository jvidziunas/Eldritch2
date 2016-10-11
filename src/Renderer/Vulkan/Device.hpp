/*==================================================================*\
  Device.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/HostAllocator.hpp>
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/HashMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			template <typename>
			class	VulkanResult;
		}
	}
}

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

	class Device {
	// - TYPE PUBLISHING ---------------------------------

	public:
		union SpecializationConstant {
			uint32_t	asUInt;
			int32_t		asInt;
			float		asFloat;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Device instance.
		Device( VkPhysicalDevice physicalDevice, Vulkan::UniquePointer<VkDevice> device, Eldritch2::Allocator& allocator );
	//!	Disable copying.
		Device( const Device& ) = delete;

		~Device() = default;

	// ---------------------------------------------------

	public:
		Vulkan::VulkanResult<VkDeviceMemory>	AllocateMemory( VkDeviceSize allocationSizeInBytes, Eldritch2::uint32 memoryTypeIndex );

		Vulkan::VulkanResult<VkCommandPool>		CreateCommandPool( VkCommandPoolCreateFlags flags, Eldritch2::uint32 queueFamilyIndex );

		Vulkan::VulkanResult<VkSemaphore>		CreateSemaphore( VkSemaphoreCreateFlags flags );

		Vulkan::VulkanResult<VkFence>			CreateFence( VkFenceCreateFlags flags );

		Vulkan::VulkanResult<VkEvent>			CreateEvent( VkEventCreateFlags flags );

	// ---------------------------------------------------

	public:
		Vulkan::HostAllocator&	GetAllocator();

	// ---------------------------------------------------

	public:
		operator	VkPhysicalDevice();

		operator	VkPipelineCache();

		operator	VkDevice();

	// ---------------------------------------------------

	//!	Disable assignment.
		Device&	operator=( const Device& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Vulkan::HostAllocator										_allocator;

		VkPhysicalDevice													_physicalDevice;
		VkQueue																_presentQueue;
		VkQueue																_sparseBindQueue;
		VkQueue																_resourceCopyQueue;

		Vulkan::UniquePointer<VkDevice>										_device;
		Vulkan::UniquePointer<VkPipelineCache>								_pipelineCache;
		Vulkan::UniquePointer<VkFence>										_presentFence;
		Vulkan::UniquePointer<VkFence>										_copyFence;
		Vulkan::UniquePointer<VkSemaphore>									_copySemaphore;
		Vulkan::UniquePointer<VkCommandPool>								_copyCommandPool;

		Eldritch2::HashMap<Eldritch2::Utf8String<>, SpecializationConstant>	_specializationConstants;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/Device.inl>
//------------------------------------------------------------------//
#if defined( CLEANUP_CREATEEVENT )
#	pragma pop_macro( "CreateEvent" )
#	undef CLEANUP_CREATEEVENT
#endif	// defined( CLEANUP_CREATEEVENT )
#if defined( CLEANUP_CREATESEMAPHORE )
#	pragma pop_macro( "CreateSemaphore" )
#	undef CLEANUP_CREATESEMAPHORE
#endif	// defined( CLEANUP_CREATESEMAPHORE )
