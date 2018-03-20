/*==================================================================*\
  ImageAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  
  
  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/AssetViews/ImageAsset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	ImageAsset::ImageAsset( const Utf8Char* const path ) : Asset( path ) {}

// ---------------------------------------------------

	ErrorCode ImageAsset::BindResources( const Builder& builder ) {
		if (Failed( CrunchImageSource::BindResources( builder.GetRawBytes() )) ) {
			builder.WriteLog( MessageType::Error, "{} contains malformed data!", GetPath() );
			return Error::Unspecified;
		}

		return Error::None;
	}

// ---------------------------------------------------

	void ImageAsset::FreeResources() {
		CrunchImageSource::FreeResources();
	}

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2