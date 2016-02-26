/*==================================================================*\
  VulkanProperties.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class PhysicalDeviceProperties {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysicalDeviceProperties instance.
		/*!	@param[in] device Vulkan physical device to pull properties from.
			*/
		PhysicalDeviceProperties( ::VkPhysicalDevice device );
		//!	Constructs this @ref PhysicalDeviceProperties instance.
		PhysicalDeviceProperties( const PhysicalDeviceProperties& ) = default;
		//!	Constructs this @ref PhysicalDeviceProperties instance.
		PhysicalDeviceProperties() = default;

		~PhysicalDeviceProperties() = default;

	// ---------------------------------------------------

		::Eldritch2::uint32	GetIndexOfLargestHeapSupportingFlags( ::VkMemoryPropertyFlags flags ) const;

	// ---------------------------------------------------

		bool	DescribesNamedDevice( const char* adapterName ) const;

	// ---------------------------------------------------

		ETInlineHint bool	IsIntegratedGpu() const;

		ETInlineHint bool	IsDiscreteGpu() const;

	// - DATA MEMBERS ------------------------------------

	private:
		::VkPhysicalDeviceProperties		_properties;
		::VkPhysicalDeviceMemoryProperties	_memoryProperties;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/VulkanProperties.inl>
//------------------------------------------------------------------//