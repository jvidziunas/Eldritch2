/*==================================================================*\
  VulkanStructures.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/List.hpp>
#include <Renderer/Vulkan/VulkanProperties.hpp>
#include <Renderer/Vulkan/VulkanResult.hpp>
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class VulkanBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref InstanceCreateInfo instance.
		VulkanBuilder( ::Eldritch2::Allocator& allocator, uint32_t vulkanApiVersion = VK_API_VERSION );

		~VulkanBuilder() = default;

	// ---------------------------------------------------

		Vulkan::VulkanResult<::VkInstance>	Create( const ::VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<const char*>	_enabledExtensions;
		::Eldritch2::ResizableArray<const char*>	_enabledLayers;

		::VkApplicationInfo							_applicationInfo;
		::VkInstanceCreateInfo						_createInfo;
	};

// ---

	class LogicalDeviceBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		LogicalDeviceBuilder( ::Eldritch2::Allocator& allocator );

		~LogicalDeviceBuilder() = default;

	// ---------------------------------------------------

		Vulkan::VulkanResult<::VkDevice>	Create( ::VkPhysicalDevice physicalDevice, const ::VkAllocationCallbacks* callbacks = nullptr );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<const char*>	_enabledExtensions;
		::Eldritch2::ResizableArray<const char*>	_enabledLayers;

		::VkDeviceCreateInfo						_createInfo;
	};

// ---

	class PhysicalDeviceEnumerator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysicalDeviceEnumerator instance.
		PhysicalDeviceEnumerator( ::VkInstance instance, ::Eldritch2::Allocator& allocator );

		~PhysicalDeviceEnumerator() = default;

	// ---------------------------------------------------

		::VkPhysicalDevice	GetTopDevice() const;

	// ---------------------------------------------------

		template <typename DeviceFilter>
		ETInlineHint PhysicalDeviceEnumerator&	FilterDevices( DeviceFilter filter );

		template <typename SortPredicate>
		ETInlineHint PhysicalDeviceEnumerator&	SortDevices( SortPredicate predicate );
	
	// - TYPE PUBLISHING ---------------------------------

	private:
		struct EnumeratedDevice {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			EnumeratedDevice( ::VkPhysicalDevice device );
			EnumeratedDevice() = default;

		// - DATA MEMBERS ------------------------------------

			::VkPhysicalDevice					device;
			Vulkan::PhysicalDeviceProperties	properties;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::List<EnumeratedDevice>	_enumeratedDevices;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/VulkanBuilders.inl>
//------------------------------------------------------------------//