/*==================================================================*\
  VirtualTexture.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/VirtualTexture.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	VirtualTexture::VirtualTexture( UniquePointer<::VkImage>&& addressTable, UniquePointer<::VkImageView>&& addressTableView, UniquePointer<::VkImage>&& backingStore, UniquePointer<::VkImageView>&& backingStoreView ) : _addressTable( ::std::move( addressTable ) ), _addressTableView( ::std::move( addressTableView ) ), _backingStore( ::std::move( backingStore ) ), _backingStoreView( ::std::move( backingStoreView ) ) {}

// ---------------------------------------------------

	VirtualTexture::VirtualTexture( UniquePointer<::VkImage>&& backingStore, UniquePointer<::VkImageView>&& backingStoreView ) : _addressTable( nullptr ), _addressTableView( nullptr ), _backingStore( ::std::move( backingStore ) ), _backingStoreView( ::std::move( backingStoreView ) ) {}


}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2