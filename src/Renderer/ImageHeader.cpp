/*==================================================================*\
  ImageHeader.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/ImageHeader.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	ImageHeader::ImageHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header ) : samplingDescriptor( samplingDescriptor ), imageClass( ImageClass::Texture2D ) {
		image.as2D = header;
	}

// ---------------------------------------------------

	ImageHeader::ImageHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header ) : samplingDescriptor( samplingDescriptor ), imageClass( ImageClass::Texture3D ) {
		image.as3D = header;
	}

// ---------------------------------------------------

	ImageHeader::ImageHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header ) : samplingDescriptor( samplingDescriptor ), imageClass( ImageClass::Cubemap ) {
		image.asCubemap = header;
	}

}	// namespace Renderer
}	// namespace Eldritch2