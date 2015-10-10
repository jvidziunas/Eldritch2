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
#include <Renderer/D3D11/Builders/Direct3D11TextureBuilder.hpp>
#include <Utility/Memory/MemStdLib.hpp>
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

	D3D11RenderTargetViewBuilder::D3D11RenderTargetViewBuilder( Direct3D11TextureBuilder& textureBuilder ) : _textureBuilder( textureBuilder ),
																											 _format( ::DXGI_FORMAT_UNKNOWN ),
																											 _initialSlice( 0u ),
																											 _sizeInSlices( 0u ),
																											 _mipIndex( 0u ) {
		textureBuilder.SetNeedsRenderTargetView( true );
	}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder::~D3D11RenderTargetViewBuilder() {}

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
		_format = format;

		return *this;
	}

// ---------------------------------------------------

	D3D11RenderTargetViewBuilder& D3D11RenderTargetViewBuilder::SetDebugName( const char* name ) {
		_debugName = name;

		return *this;
	}

// ---------------------------------------------------

	ErrorCode D3D11RenderTargetViewBuilder::Compile( ::ID3D11Device* const device ) {
		struct Texture1DDescriptor : public ::D3D11_TEX1D_RTV {
			ETInlineHint Texture1DDescriptor( const D3D11RenderTargetViewBuilder& builder ) {
				MipSlice	= builder._mipIndex;
			}
		};

	// ---------------------------------------------------

		struct Texture1DArrayDescriptor : public ::D3D11_TEX1D_ARRAY_RTV {
			ETInlineHint Texture1DArrayDescriptor( const D3D11RenderTargetViewBuilder& builder ) {
				MipSlice			= builder._mipIndex;
				FirstArraySlice		= builder._initialSlice;
				ArraySize			= builder._sizeInSlices;
			}
		};

	// ---------------------------------------------------

		struct Texture2DDescriptor : public ::D3D11_TEX2D_RTV {
			ETInlineHint Texture2DDescriptor( const D3D11RenderTargetViewBuilder& builder ) {
				MipSlice	= builder._mipIndex;
			}
		};

	// ---------------------------------------------------

		struct Texture2DArrayDescriptor : public ::D3D11_TEX2D_ARRAY_RTV {
			ETInlineHint Texture2DArrayDescriptor( const D3D11RenderTargetViewBuilder& builder ) {
				MipSlice		= builder._mipIndex;
				FirstArraySlice	= builder._initialSlice;
				ArraySize		= builder._sizeInSlices;
			}
		};

	// ---------------------------------------------------

		struct MSAATexture2DArrayDescriptor : public ::D3D11_TEX2DMS_ARRAY_RTV {
			ETInlineHint MSAATexture2DArrayDescriptor( const D3D11RenderTargetViewBuilder& builder ) {
				FirstArraySlice	= builder._initialSlice;
				ArraySize		= builder._sizeInSlices;
			}
		};

	// ---------------------------------------------------

		struct Texture3DDescriptor : public ::D3D11_TEX3D_RTV {
			ETInlineHint Texture3DDescriptor( const D3D11RenderTargetViewBuilder& builder ) {
				MipSlice	= builder._mipIndex;
				FirstWSlice	= builder._initialSlice;
				WSize		= builder._sizeInSlices;
			}
		};

	// ---

		::D3D11_RENDER_TARGET_VIEW_DESC	viewDescriptor;
		const bool						isArrayResource( _textureBuilder._arraySize != 1u );
		const bool						isMSAAResource( _textureBuilder._sampleDesc.Count != 1u );

		viewDescriptor.Format = _format;

		switch( _textureBuilder._type ) {
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
				if( isArrayResource ) {
					viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
					viewDescriptor.Texture1DArray	= Texture1DArrayDescriptor( *this );
				} else {	// if( isArrayResource )
					viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE1D;
					viewDescriptor.Texture1D		= Texture1DDescriptor( *this );
				}
				break;
			}	// case ::D3D11_RESOURCE_DIMENSION_TEXTURE1D
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				if( isArrayResource ) {
					if( isMSAAResource ) {
						viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
						viewDescriptor.Texture2DMSArray	= MSAATexture2DArrayDescriptor( *this );
					} else {	// if( isMSAAResource )
						viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
						viewDescriptor.Texture2DArray	= Texture2DArrayDescriptor( *this );
					}
				} else {	// if( isArrayResource )
					if( isMSAAResource ) {
						viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE2DMS;
						// No configurable parameters for MSAA textures.
					} else {	// if( isMSAAResource )
						viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE2D;
						viewDescriptor.Texture2D		= Texture2DDescriptor( *this );
					}
				}
				break;
			}	// case ::D3D11_RESOURCE_DIMENSION_TEXTURE2D
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				viewDescriptor.ViewDimension	= ::D3D11_RTV_DIMENSION_TEXTURE3D;
				viewDescriptor.Texture3D		= Texture3DDescriptor( *this );
				break;
			}	// case ::D3D11_RESOURCE_DIMENSION_TEXTURE3D
			default: {
				ETNoDefaultCaseHint;
			}
		};

		::HRESULT	nativeResult( device->CreateRenderTargetView( _textureBuilder.GetTexture().GetUnadornedPointer(), &viewDescriptor, _view.GetInterfacePointer() ) );

		if( _debugName && _view ) {
			_view->SetPrivateData( ::WKPDID_D3DDebugObjectName, static_cast<::UINT>( StringLength( _debugName ) ), _debugName );
		}

		return SUCCEEDED( nativeResult ) ? Errors::NONE : Errors::UNSPECIFIED;
	}

// ---------------------------------------------------

	ErrorCode D3D11RenderTargetViewBuilder::Compile( const COMPointer<ID3D11Device>& device ) {
		return this->Compile( device.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	const COMPointer<::ID3D11RenderTargetView>& D3D11RenderTargetViewBuilder::GetView() const {
		return _view;
	}

}	// namespace Renderer
}	// namespace Eldritch2