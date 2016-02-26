/*==================================================================*\
  SmartPointers.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace Detail {

	class InstanceDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceDeleterBase instance.
		InstanceDeleterBase( const ::VkAllocationCallbacks* const callbacks = nullptr );
		//!	Constructs this @ref InstanceDeleterBase instance.
		InstanceDeleterBase( const InstanceDeleterBase& ) = default;

		~InstanceDeleterBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		const ::VkAllocationCallbacks*	_allocationCallbacks;
	};

// ---

	class DeviceChildDeleterBase : public InstanceDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref DeviceChildDeleterBase instance.
		DeviceChildDeleterBase( const ::VkDevice device, const ::VkAllocationCallbacks* const callbacks = nullptr ) : InstanceDeleterBase( callbacks ), _device( device ) {}
		//!	Constructs this @ref DeviceChildDeleterBase instance.
		DeviceChildDeleterBase( const DeviceChildDeleterBase& ) = default;
		//!	Constructs this @ref DeviceChildDeleterBase instance.
		DeviceChildDeleterBase() = default;

		~DeviceChildDeleterBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		::VkDevice	_device;
	};

// ---

	template <typename VulkanHandle>
	class InstanceDeleter {
	public:
		void operator()( VulkanHandle handle ) {
			static_assert(false, "Vulkan::Detail::InstanceDeleter needs specialization for this type!");
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkInstance> : public InstanceDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const ::VkAllocationCallbacks* const callbacks = nullptr ) : InstanceDeleterBase( callbacks ) {}
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

		void operator()( ::VkInstance instance ) {
			::vkDestroyInstance( instance, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkDevice> : public InstanceDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const ::VkAllocationCallbacks* const callbacks = nullptr ) : InstanceDeleterBase( callbacks ) {}
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

		void operator()( ::VkDevice device ) {
			::vkDestroyDevice( device, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkPipeline> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const ::VkDevice device, const ::VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

		void operator()( ::VkPipeline pipeline ) {
			::vkDestroyPipeline( _device, pipeline, _allocationCallbacks );
		}
	};

}	// namespace Detail
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

