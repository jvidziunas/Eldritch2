/*==================================================================*\
  VirtualTexture.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class VirtualTexture {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		VirtualTexture( Vulkan::UniquePointer<::VkImage>&& addressTable, Vulkan::UniquePointer<::VkImageView>&& addressTableView,
						Vulkan::UniquePointer<::VkImage>&& backingStore, Vulkan::UniquePointer<::VkImageView>&& backingStoreView );
		VirtualTexture( Vulkan::UniquePointer<::VkImage>&& backingStore, Vulkan::UniquePointer<::VkImageView>&& backingStoreView );
		VirtualTexture( const VirtualTexture& ) = delete;

		~VirtualTexture() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<::VkImage>		_addressTable;
		Vulkan::UniquePointer<::VkImageView>	_addressTableView;

		Vulkan::UniquePointer<::VkImage>		_backingStore;
		Vulkan::UniquePointer<::VkImageView>	_backingStoreView;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2