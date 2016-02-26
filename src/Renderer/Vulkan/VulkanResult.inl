/*==================================================================*\
  VulkanResult.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::VulkanResult( Vulkan::UniquePointer<VulkanObject>&& pointer ) : object( ::std::move( pointer ) ), result( VK_SUCCESS ) {}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::VulkanResult( const ::VkResult result ) : object( nullptr ), result( result ) {}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::operator bool() const {
		return result == VK_SUCCESS;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

