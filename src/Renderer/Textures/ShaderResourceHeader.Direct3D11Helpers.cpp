/*==================================================================*\
  ShaderResourceHeader.Direct3D11Helpers.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/builders/Direct3D11TextureBuilder.hpp>
#include <Renderer/Textures/ShaderResourceHeader.hpp>
#include <Renderer/D3D11/D3D11FormatHelpers.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	void ShaderResourceHeader::SamplingDescriptor::ConfigureResourceBuilder( Direct3D11TextureBuilder& builder ) {
		builder.SetFormat( BuildDXGIFormat( layout, storageFormat, filteringFlags ) );
	}

// ---------------------------------------------------

	void ShaderResourceHeader::Header2D::ConfigureResourceBuilder( Direct3D11TextureBuilder& builder ) {
		builder.SetTexture2D( topLevelWidthInPixels, topLevelHeightInPixels, arraySizeInSlices ).SetMSAAProperties( 1u, 0u );
	}

// ---------------------------------------------------

	void ShaderResourceHeader::Header3D::ConfigureResourceBuilder( Direct3D11TextureBuilder& builder ) {
		builder.SetTexture3D( topLevelWidthInPixels, topLevelHeightInPixels, topLevelDepthInPixels );
	}

// ---------------------------------------------------

	void ShaderResourceHeader::HeaderCubemap::ConfigureResourceBuilder( Direct3D11TextureBuilder& builder ) {
		builder.SetCubemap( edgeSizeInPixels, edgeSizeInPixels, arraySizeInSlices ).SetMSAAProperties( 1u, 0u );
	}

}	// namespace Renderer
}	// namespace Eldritch2



