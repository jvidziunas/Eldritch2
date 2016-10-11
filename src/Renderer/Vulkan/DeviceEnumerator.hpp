/*==================================================================*\
  DeviceEnumerator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//
#include <functional>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			class	PhysicalDeviceProperties;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class DeviceEnumerator {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using OrderingPredicate = std::function<bool ( const Vulkan::PhysicalDeviceProperties&, const Vulkan::PhysicalDeviceProperties& )>;
		using Filter			= std::function<bool ( const Vulkan::PhysicalDeviceProperties& )>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref DeviceEnumerator instance.
		DeviceEnumerator( VkInstance instance, Eldritch2::Allocator& allocator );

		~DeviceEnumerator() = default;

	// ---------------------------------------------------

	public:
		VkPhysicalDevice	GetTopDevice() const;

	// ---------------------------------------------------

	public:
		DeviceEnumerator&	OrderBy( OrderingPredicate&& predicate );

		DeviceEnumerator&	FilterUnless( Filter&& filter );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::ResizableArray<Filter>	_filters;
		VkInstance							_instance;
		OrderingPredicate					_orderingPredicate;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/DeviceEnumerator.inl>
//------------------------------------------------------------------//