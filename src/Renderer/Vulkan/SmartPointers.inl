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

	class DeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DeleterBase instance.
		DeleterBase( const VkAllocationCallbacks* const callbacks );
	//!	Constructs this @ref DeleterBase instance.
		DeleterBase( const DeleterBase& ) = default;
	//!	Constructs this @ref DeleterBase instance.
		DeleterBase() = default;

		~DeleterBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		const VkAllocationCallbacks*	_allocationCallbacks;
	};

// ---

	class InstanceChildDeleterBase : public DeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceChildDeleterBase instance.
		InstanceChildDeleterBase( VkInstance instance, const VkAllocationCallbacks* const callbacks = nullptr ) : DeleterBase( callbacks ), _instance( instance ) {}
	//!	Constructs this @ref InstanceChildDeleterBase instance.
		InstanceChildDeleterBase( const InstanceChildDeleterBase& ) = default;
	//!	Constructs this @ref InstanceChildDeleterBase instance.
		InstanceChildDeleterBase() = default;

		~InstanceChildDeleterBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		VkInstance	_instance;
	};

// ---

	class DeviceChildDeleterBase : public DeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DeviceChildDeleterBase instance.
		DeviceChildDeleterBase( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeleterBase( callbacks ), _device( device ) {}
	//!	Constructs this @ref DeviceChildDeleterBase instance.
		DeviceChildDeleterBase( const DeviceChildDeleterBase& ) = default;
	//!	Constructs this @ref DeviceChildDeleterBase instance.
		DeviceChildDeleterBase() = default;

		~DeviceChildDeleterBase() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		VkDevice	_device;
	};

// ---

	template <typename VulkanHandle>
	class InstanceDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------
	public:
		InstanceDeleter() {
			static_assert( false, "Vulkan::Detail::InstanceDeleter needs specialization for this type!" );
		}

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VulkanHandle /*handle*/ ) {}
	};

// ---

	template <>
	class InstanceDeleter<VkInstance> : public DeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const VkAllocationCallbacks* const callbacks ) : DeleterBase( callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkInstance instance ) {
			vkDestroyInstance( instance, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkDevice> : public DeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const VkAllocationCallbacks* const callbacks ) : DeleterBase( callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkDevice device ) {
			vkDestroyDevice( device, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkSemaphore> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkSemaphore fence ) {
			vkDestroySemaphore( _device, fence, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkFence> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkFence fence ) {
			vkDestroyFence( _device, fence, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkEvent> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkEvent event ) {
			vkDestroyEvent( _device, event, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkDeviceMemory> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkDeviceMemory commandPool ) {
			vkFreeMemory( _device, commandPool, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkCommandPool> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkCommandPool commandPool ) {
			vkDestroyCommandPool( _device, commandPool, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkPipeline> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkPipeline pipeline ) {
			vkDestroyPipeline( _device, pipeline, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkBuffer> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkBuffer buffer ) {
			vkDestroyBuffer( _device, buffer, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkBufferView> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkBufferView view ) {
			vkDestroyBufferView( _device, view, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkImage> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkImage image ) {
			vkDestroyImage( _device, image, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkImageView> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkImageView image ) {
			vkDestroyImageView( _device, image, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkPipelineCache> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkPipelineCache cache ) {
			vkDestroyPipelineCache( _device, cache, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkShaderModule> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkShaderModule module ) {
			vkDestroyShaderModule( _device, module, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkRenderPass> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkRenderPass pass ) {
			vkDestroyRenderPass( _device, pass, _allocationCallbacks );
		}
	};

// ---

	template <>
	class InstanceDeleter<VkFramebuffer> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkFramebuffer framebuffer ) {
			vkDestroyFramebuffer( _device, framebuffer, _allocationCallbacks );
		}
	};

// ---

#if defined( VK_EXT_debug_report )
	template <>
	class InstanceDeleter<VkDebugReportCallbackEXT> : public DeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkInstance instance, const VkAllocationCallbacks* const callbacks ) : DeleterBase( callbacks ), _instance( instance ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkDebugReportCallbackEXT callback ) {
			reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr( _instance, "vkDestroyDebugReportCallbackEXT" ))( _instance, callback, _allocationCallbacks );
		}

	// - DATA MEMBERS ------------------------------------

	protected:
		VkInstance	_instance;
	};
#endif	// defined( VK_EXT_debug_report )

// ---

#if defined( VK_KHR_swapchain )
	template <>
	class InstanceDeleter<VkSwapchainKHR> : public DeviceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkDevice device, const VkAllocationCallbacks* const callbacks = nullptr ) : DeviceChildDeleterBase( device, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkSwapchainKHR swapChain ) {
			vkDestroySwapchainKHR( _device, swapChain, _allocationCallbacks );
		}
	};
#endif

// ---

#if defined( VK_KHR_surface )
	template <>
	class InstanceDeleter<VkSurfaceKHR> : public InstanceChildDeleterBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( VkInstance instance, const VkAllocationCallbacks* const callbacks = nullptr ) : InstanceChildDeleterBase( instance, callbacks ) {}
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter( const InstanceDeleter& ) = default;
	//!	Constructs this @ref InstanceDeleter instance.
		InstanceDeleter() = default;

		~InstanceDeleter() = default;

	// ---------------------------------------------------

	public:
		void operator()( VkSurfaceKHR surface ) {
			vkDestroySurfaceKHR( _instance, surface, _allocationCallbacks );
		}
	};
#endif

}	// namespace Detail
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

