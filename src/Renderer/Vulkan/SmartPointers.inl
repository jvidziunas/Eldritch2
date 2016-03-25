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

	class InstanceChildDeleterBase : public InstanceDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceChildDeleterBase instance.
		InstanceChildDeleterBase( const ::VkInstance instance, const ::VkAllocationCallbacks* const callbacks = nullptr ) : InstanceDeleterBase( callbacks ), _instance( instance ) {}
		//!	Constructs this @ref InstanceChildDeleterBase instance.
		InstanceChildDeleterBase( const InstanceChildDeleterBase& ) = default;
		//!	Constructs this @ref InstanceChildDeleterBase instance.
		InstanceChildDeleterBase() = default;

		~InstanceChildDeleterBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		::VkInstance	_instance;
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
	class InstanceDeleter<::VkDeviceMemory> : public DeviceChildDeleterBase {
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

		void operator()( ::VkDeviceMemory deviceMemory ) {
			::vkFreeMemory( _device, deviceMemory, _allocationCallbacks );
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

// ---

	template <>
	class InstanceDeleter<::VkBuffer> : public DeviceChildDeleterBase {
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

		void operator()( ::VkBuffer buffer ) {
			::vkDestroyBuffer( _device, buffer, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkImage> : public DeviceChildDeleterBase {
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

		void operator()( ::VkImage image ) {
			::vkDestroyImage( _device, image, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkFence> : public DeviceChildDeleterBase {
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

		void operator()( ::VkFence fence ) {
			::vkDestroyFence( _device, fence, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkBufferView> : public DeviceChildDeleterBase {
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

		void operator()( ::VkBufferView view ) {
			::vkDestroyBufferView( _device, view, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkImageView> : public DeviceChildDeleterBase {
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

		void operator()( ::VkImageView image ) {
			::vkDestroyImageView( _device, image, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<::VkSurfaceKHR> : public InstanceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const ::VkInstance instance, const ::VkAllocationCallbacks* const callbacks = nullptr ) : InstanceChildDeleterBase( instance, callbacks ) {}
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
		//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

		void operator()( ::VkSurfaceKHR surface ) {
			::vkDestroySurfaceKHR( _instance, surface, _allocationCallbacks );
		}
	};

}	// namespace Detail
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

