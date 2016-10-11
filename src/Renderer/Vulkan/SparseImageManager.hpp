/*==================================================================*\
  SparseTextureFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SparseImage.hpp>
#include <Utility/Containers/HashMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class SparseImageManager {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SparseImageManager instance.
		SparseImageManager( Eldritch2::Allocator& allocator );
	//!	Disable copying.
		SparseImageManager( const SparseImageManager& ) = delete;

		~SparseImageManager() = default;

	// ---------------------------------------------------

	public:
		Vulkan::SparseImage&	GetImageForDevice( VkDevice device );

	// ---------------------------------------------------

	//!	Disable assignment.
		SparseImageManager&	operator=( const SparseImageManager& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::HashMap<VkDevice, Vulkan::SparseImage>	_images;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2