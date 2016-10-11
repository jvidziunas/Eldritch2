/*==================================================================*\
  SparseImageManager.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SparseImageManager.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	SparseImageManager::SparseImageManager( Allocator& allocator ) : _images( { allocator, "Vulkan Sparse Image Manager Directory Allocator" } ) {}

// ---------------------------------------------------

	SparseImage& SparseImageManager::GetImageForDevice( VkDevice device ) {
		const auto	candidate( _images.Find( device ) );

		return candidate->second;
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

