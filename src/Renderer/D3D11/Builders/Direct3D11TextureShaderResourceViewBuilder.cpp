/*==================================================================*\
  Direct3D11TextureShaderResourceViewBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/Builders/Direct3D11TextureShaderResourceViewBuilder.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace {

	enum : ::UINT {
		ALL_SLICES_IN_RESOURCE	= static_cast<::UINT>(-1),
		LAST_MIP_IN_RESOURCE	= static_cast<::UINT>(-1)
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {

	Direct3D11TextureShaderResourceViewBuilder::Direct3D11TextureShaderResourceViewBuilder() : _formatOverride( ::DXGI_FORMAT_UNKNOWN ),
																							   _initialSlice( 0u ),
																							   _sizeInSlices( ALL_SLICES_IN_RESOURCE ),
																							   _firstMipIndex( 0u ),
																							   _lastMipIndex( LAST_MIP_IN_RESOURCE ),
																							   _debugName( nullptr ) {}

// ---------------------------------------------------

	COMPointer<::ID3D11ShaderResourceView> Direct3D11TextureShaderResourceViewBuilder::Compile( const COMPointer<::ID3D11Resource>& resource ) {
		return Compile( resource.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	COMPointer<::ID3D11ShaderResourceView> Direct3D11TextureShaderResourceViewBuilder::Compile( ::ID3D11Resource* const resource ) {
		return nullptr;
	}

// ---------------------------------------------------

	COMPointer<::ID3D11ShaderResourceView> Direct3D11TextureShaderResourceViewBuilder::Compile( const COMPointer<::ID3D11Texture2D>& resource ) {
		return Compile( resource.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	COMPointer<::ID3D11ShaderResourceView> Direct3D11TextureShaderResourceViewBuilder::Compile( ::ID3D11Texture2D* const resource ) {
		return nullptr;
	}

// ---------------------------------------------------

	COMPointer<::ID3D11ShaderResourceView> Direct3D11TextureShaderResourceViewBuilder::Compile( const COMPointer<::ID3D11Texture3D>& resource ) {
		return Compile( resource.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	COMPointer<::ID3D11ShaderResourceView> Direct3D11TextureShaderResourceViewBuilder::Compile( ::ID3D11Texture3D* const resource ) {
		return nullptr;
	}

}	// namespace Renderer
}	// namespace Eldritch2



