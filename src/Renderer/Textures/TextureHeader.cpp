/*==================================================================*\
  TextureHeader.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Textures/TextureHeader.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	TextureHeader::TextureHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header ) : samplingDescriptor( samplingDescriptor ), headerClass( Class::Texture2D ) {
		headers.as2D = header;
	}

// ---------------------------------------------------

	TextureHeader::TextureHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header ) : samplingDescriptor( samplingDescriptor ), headerClass( Class::Texture3D ) {
		headers.as3D = header;
	}

// ---------------------------------------------------

	TextureHeader::TextureHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header ) : samplingDescriptor( samplingDescriptor ), headerClass( Class::Cubemap ) {
		headers.asCubemap = header;
	}

}	// namespace Renderer
}	// namespace Eldritch2