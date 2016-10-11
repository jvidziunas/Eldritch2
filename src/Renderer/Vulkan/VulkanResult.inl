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
	ETInlineHint VulkanResult<VulkanObject>::VulkanResult( Vulkan::UniquePointer<VulkanObject> object ) : _object( eastl::move( object ) ), _result( VkResult::VK_SUCCESS ) {}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::VulkanResult( VkResult result ) : _object( nullptr ), _result( result ) {}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::VulkanResult( VulkanResult<VulkanObject>&& source ) : _object( eastl::move( source._object ) ), _result( source._result ) {}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint const Vulkan::UniquePointer<VulkanObject>& VulkanResult<VulkanObject>::operator*() const {
		return _object;
	}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint Vulkan::UniquePointer<VulkanObject>& VulkanResult<VulkanObject>::operator*() {
		return _object;
	}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint const Vulkan::UniquePointer<VulkanObject>* VulkanResult<VulkanObject>::operator->() const {
		return &_object;
	}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint Vulkan::UniquePointer<VulkanObject>* VulkanResult<VulkanObject>::operator->() {
		return &_object;
	}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::operator VkResult() const {
		return _result;
	}

// ---------------------------------------------------

	template <typename VulkanObject>
	ETInlineHint VulkanResult<VulkanObject>::operator bool() const {
		return VkResult::VK_SUCCESS <= _result;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

