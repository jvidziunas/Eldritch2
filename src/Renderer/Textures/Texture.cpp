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
#include <Renderer/Textures/TextureUtilities.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Renderer/Textures/Texture.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	enum : uint32 {
		NUM_CUBEMAP_FACES = 6u
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {

	Texture::Texture() {}

// ---------------------------------------------------

	Texture::~Texture() {}

// ---------------------------------------------------

	uint32 Texture::GetWidthInTexels( uint32 MIPLevel ) const {
		return Max( _version1Header.widthInTexels >> MIPLevel, 1u );
	}

// ---------------------------------------------------

	uint32 Texture::GetHeightInTexels( uint32 MIPLevel ) const {
		return Max( _version1Header.heightInTexels >> MIPLevel, 1u );
	}

// ---------------------------------------------------

	uint32 Texture::GetDepthInTexels( uint32 MIPLevel ) const {
		return TextureType::TEXTURE_3D == GetType() ? Max( _version1Header.depthInTexels >> MIPLevel, 1u ) : 1u;
	}

// ---------------------------------------------------

	uint32 Texture::GetArraySize() const {
		return TextureType::TEXTURE_3D != GetType() ? _version1Header.arraySlices : 1u;
	}

// ---------------------------------------------------

	uint32 Texture::CalcActualMIPLevelsForCompleteChain() const {
		return CalcTotalMIPPyramidLevels( GetWidthInTexels(), GetHeightInTexels(), GetDepthInTexels() );
	}

// ---------------------------------------------------

	uint32 Texture::GetTotalMIPCount() const {
		return _version1Header.totalMIPCount;
	}

// ---------------------------------------------------

	uint32 Texture::GetPresentMIPOffset() const {
		return _version1Header.presentMIPOffset;
	}

// ---------------------------------------------------

	uint32 Texture::GetPresentMIPCount() const {
		return _version1Header.presentMIPCount;
	}

// ---------------------------------------------------

	GPUSamplingDescriptor Texture::GetSamplingDescriptor() const {
		return _version1Header.samplingDescriptor;
	}

// ---------------------------------------------------

	Renderer::TextureType Texture::GetType() const {
		return _version1Header.textureType > TextureType::MALFORMED ? TextureType::MALFORMED : _version1Header.textureType;
	}

// ---------------------------------------------------

	bool Texture::ContainsCompleteMIPChain() const {
		return( 0u == GetPresentMIPOffset() );
	}

// ---------------------------------------------------

	bool Texture::HasValidFourCC() const {
		return( Texture::CONTENT_FOURCC == _basicHeader.fourCC );
	}

// ---------------------------------------------------

	const void* Texture::GetData() const {
		return static_cast<const void*>( _version1Header.textureData );
	}

}	// namespace Renderer
}	// namespace Eldritch2