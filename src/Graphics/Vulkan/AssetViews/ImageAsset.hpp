/*==================================================================*\
  ImageAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/CrunchImageSource.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	class ImageAsset : public Assets::Asset, public CrunchImageSource {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageAsset instance.
	/*!	@param[in] path Null-terminated, UTF-8-encoded file system path to the asset this @ref ImageAsset describes. */
		ImageAsset( const Utf8Char* const path );
	//!	Disable copy construction.
		ImageAsset( const ImageAsset& ) = delete;

		~ImageAsset() override = default;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ImageAsset&	operator=( const ImageAsset& ) = delete;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2