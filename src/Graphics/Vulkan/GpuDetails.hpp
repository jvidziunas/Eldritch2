/*==================================================================*\
  GpuDetails.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class GpuDetails {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GpuDetails instance.
	/*!	@param[in] gpu Vulkan physical device to pull properties from. */
		GpuDetails( VkPhysicalDevice gpu );
	//!	Constructs this @ref GpuDetails instance.
		GpuDetails( const GpuDetails& ) = default;
	//!	Constructs this @ref GpuDetails instance.
		GpuDetails() = default;

		~GpuDetails() = default;

	// ---------------------------------------------------

	public:
		uint32	GetIndexOfLargestHeapSupportingFlags( VkMemoryPropertyFlags flags ) const;

	// ---------------------------------------------------

	public:
		bool	DescribesNamedGpu( const char* name ) const;

	// ---------------------------------------------------

	public:
		bool	IsIntegrated() const;

		bool	IsDiscrete() const;

	// - DATA MEMBERS ------------------------------------

	public:
		VkPhysicalDeviceProperties			basic;
		VkPhysicalDeviceMemoryProperties	memory;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/GpuDetails.inl>
//------------------------------------------------------------------//