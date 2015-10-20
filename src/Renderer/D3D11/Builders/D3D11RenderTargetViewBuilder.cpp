/*==================================================================*\
  Direct3D11TextureBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/Builders/D3D11RenderTargetViewBuilder.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	D3D11RenderTargetViewBuilder::D3D11RenderTargetViewBuilder() : _formatOverride( ::DXGI_FORMAT_UNKNOWN ), _initialSlice( 0u ), _sizeInSlices( static_cast<::UINT>(-1) ), _mipIndex( 0u ) {}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder& D3D11RenderTargetViewBuilder::SetDesiredSlices( uint32 initialArraySlice, uint32 arraySizeInSlices ) {
		_initialSlice	= initialArraySlice;
		_sizeInSlices	= arraySizeInSlices;

		return *this;
	}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder& D3D11RenderTargetViewBuilder::SetDesiredSlice( uint32 arraySlice ) {
		_initialSlice	= arraySlice;
		_sizeInSlices	= 1u;

		return *this;
	}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder& D3D11RenderTargetViewBuilder::SetMIPMapIndex( uint32 index ) {
		_mipIndex = index;

		return *this;
	}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder& D3D11RenderTargetViewBuilder::OverrideFormat( ::DXGI_FORMAT format ) {
		_formatOverride = format;

		return *this;
	}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder& D3D11RenderTargetViewBuilder::SetDebugName( const char* name ) {
		_debugName = name;

		return *this;
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> D3D11RenderTargetViewBuilder::Compile( const COMPointer<::ID3D11Resource>& targetResource ) {
		return Compile( targetResource.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> D3D11RenderTargetViewBuilder::Compile( ::ID3D11Resource* const targetResource ) {
		::D3D11_RESOURCE_DIMENSION	dimension;

		targetResource->GetType( &dimension );

		switch( dimension ) {
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				return Compile( static_cast<::ID3D11Texture2D*>(targetResource) );
			}

		// ---
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				return Compile( static_cast<::ID3D11Texture3D*>(targetResource) );
			}

		// ---

			default: {
				return nullptr;
			}
		}	// switch( dimension )
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> D3D11RenderTargetViewBuilder::Compile( const COMPointer<::ID3D11Texture2D>& targetResource ) {
		return Compile( targetResource.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> D3D11RenderTargetViewBuilder::Compile( ::ID3D11Texture2D* const targetResource ) {
		const ::D3D11_RTV_DIMENSION			dimension( _sizeInSlices == 1u ? ::D3D11_RTV_DIMENSION_TEXTURE2D : ::D3D11_RTV_DIMENSION_TEXTURE2DARRAY );
		::CD3D11_RENDER_TARGET_VIEW_DESC	descriptor( targetResource, dimension, _formatOverride, _mipIndex, _initialSlice, _sizeInSlices );
		COMPointer<::ID3D11Device>			device;
		::ID3D11RenderTargetView*			view( nullptr );

		targetResource->GetDevice( device.GetInterfacePointer() );

		if( FAILED( device->CreateRenderTargetView( targetResource, &descriptor, &view ) ) ) {
			return nullptr;
		}

		if( _debugName ) {
			view->SetPrivateData( ::WKPDID_D3DDebugObjectName, static_cast<::UINT>(StringLength( _debugName )), _debugName );
		}

		return { view, ::Eldritch2::PassthroughReferenceCountingSemantics };
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> D3D11RenderTargetViewBuilder::Compile( const COMPointer<::ID3D11Texture3D>& targetResource ) {
		return Compile( targetResource.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	COMPointer<::ID3D11RenderTargetView> D3D11RenderTargetViewBuilder::Compile( ::ID3D11Texture3D* const targetResource ) {
		::CD3D11_RENDER_TARGET_VIEW_DESC	descriptor( targetResource, _formatOverride, _mipIndex, _initialSlice, _sizeInSlices );
		COMPointer<::ID3D11Device>			device;
		::ID3D11RenderTargetView*			view( nullptr );

		targetResource->GetDevice( device.GetInterfacePointer() );

		if( FAILED( device->CreateRenderTargetView( targetResource, &descriptor, &view ) ) ) {
			return nullptr;
		}

		if( _debugName ) {
			view->SetPrivateData( ::WKPDID_D3DDebugObjectName, static_cast<::UINT>(StringLength( _debugName )), _debugName );
		}

		return { view, ::Eldritch2::PassthroughReferenceCountingSemantics };
	}

}	// namespace Renderer
}	// namespace Eldritch2