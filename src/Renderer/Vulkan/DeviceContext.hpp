/*==================================================================*\
  DeviceContext.hpp
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
#include <Renderer/Vulkan/VulkanResult.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class DeviceContext {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		DeviceContext( Vulkan::UniquePointer<::VkDevice>&& device, ::Eldritch2::Allocator& allocator );

		~DeviceContext() = default;

	// ---------------------------------------------------

		Vulkan::VulkanResult<::VkDeviceMemory>	AllocateMemory( ::VkDeviceSize allocationSizeInBytes, ::Eldritch2::uint32 memoryTypeIndex );

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<::VkDevice>	_device;
		Vulkan::HostAllocator				_allocator;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

