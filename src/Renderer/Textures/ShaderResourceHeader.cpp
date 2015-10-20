/*==================================================================*\
  Texture.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Textures/ShaderResourceHeader.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	ShaderResourceHeader::ShaderResourceHeader( const SamplingDescriptor& samplingDescriptor, const Header2D& header ) : _samplingDescriptor( samplingDescriptor ), _class( Class::TEXTURE_2D ) {
		_headers.header2D = header;
	}

// ---------------------------------------------------

	ShaderResourceHeader::ShaderResourceHeader( const SamplingDescriptor& samplingDescriptor, const Header3D& header ) : _samplingDescriptor( samplingDescriptor ), _class( Class::TEXTURE_3D ) {
		_headers.header3D = header;
	}

// ---------------------------------------------------

	ShaderResourceHeader::ShaderResourceHeader( const SamplingDescriptor& samplingDescriptor, const HeaderCubemap& header ) : _samplingDescriptor( samplingDescriptor ), _class( Class::CUBEMAP ) {
		_headers.headerCubemap = header;
	}

}	// namespace Renderer
}	// namespace Eldritch2