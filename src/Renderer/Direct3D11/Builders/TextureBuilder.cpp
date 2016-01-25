/*==================================================================*\
  TextureBuilder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/Builders/TextureBuilder.hpp>
#include <Renderer/Direct3D/FormatHelpers.hpp>
#include <Renderer/Textures/TextureHeader.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Memory/Allocator.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
ET_LINK_LIBRARY( "DXGUID.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace {

	static ETForceInlineHint ETNoAliasHint void UpdateFlag( ::UINT& flags, const ::UINT flag, bool enabled ) {
		const ::UINT newValue( ( flags | static_cast<::UINT>( enabled ? flag : 0u ) ) & ~( enabled ? flag : 0u ) );
		flags = newValue;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	TextureBuilder::TextureBuilder( const COMPointer<::ID3D11Device>& device ) : _device( device ),
																				 _depthInTexels( 1u ),
																				 _mipLevels( 1u ),
																				 _format( ::DXGI_FORMAT_UNKNOWN ),
																				 _usage( ::D3D11_USAGE_DEFAULT ),
																				 _bindFlags( 0u ),
																				 _accessFlags( 0u ),
																				 _miscFlags( 0u ),
																				 _debugName( nullptr ) {
		SetTexture2D( 1u, 1u, 1u );

		_sampleDesc.Count	= 1u;
		_sampleDesc.Quality	= 0u;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::ConfigureFromHeader( const TextureHeader& baseHeader ) {
		using HeaderClass = TextureHeader::Class;

	// ---

		{	const auto&	samplingDescriptor( baseHeader.samplingDescriptor );
			SetFormat( Direct3D::BuildDXGIFormat( samplingDescriptor.layout, samplingDescriptor.storageFormat, samplingDescriptor.filteringFlags ) );
		}

		switch( baseHeader.headerClass ) {
			case HeaderClass::Texture2D: {
				const auto&	header( baseHeader.headers.as2D );
				SetTexture2D( header.topLevelWidthInPixels, header.topLevelHeightInPixels, header.arraySizeInSlices );
				break;
			}	// case HeaderClass::Texture2D

			case HeaderClass::Texture3D: {
				const auto&	header( baseHeader.headers.as3D );
				SetTexture2D( header.topLevelWidthInPixels, header.topLevelHeightInPixels, header.topLevelHeightInPixels );
				break;
			}	// case HeaderClass::Texture3D

			case HeaderClass::Cubemap: {
				const auto&	header( baseHeader.headers.asCubemap );
				SetCubemap( header.edgeSizeInPixels, header.edgeSizeInPixels, header.arraySizeInCubemaps );
				break;
			}	// case HeaderClass::Cubemap
		}
		
		return SetMSAAProperties( 1u, 0u ).SetNeedsShaderResourceView();
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetTexture2D( uint32 widthInTexels, uint32 heightInTexels, uint32 arraySlices ) {
		_type			= ::D3D11_RESOURCE_DIMENSION_TEXTURE2D;

		_widthInTexels	= Max( widthInTexels,	1u );
		_heightInTexels	= Max( heightInTexels,	1u );
		_arraySize		= Max( arraySlices,		1u );

		// We're not a cubemap. Clear the flag in case it was set previously.
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_TEXTURECUBE, false );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetTexture3D( uint32 widthInTexels, uint32 heightInTexels, uint32 depthInTexels ) {
		_type			= ::D3D11_RESOURCE_DIMENSION_TEXTURE3D;

		_widthInTexels	= Max( widthInTexels,	1u );
		_heightInTexels	= Max( heightInTexels,	1u );
		_depthInTexels	= Max( depthInTexels,	1u );
		// Volume textures cannot be part of an array.
		_arraySize		= 1u;

		// We're not a cubemap. Clear the flag in case it was set previously.
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_TEXTURECUBE, false );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetCubemap( uint32 widthInTexels, uint32 heightInTexels, uint32 cubemapArraySizeInCubemaps ) {
		_type			= ::D3D11_RESOURCE_DIMENSION_TEXTURE2D;

		_widthInTexels	= Max( widthInTexels,				1u );
		_heightInTexels	= Max( heightInTexels,				1u );
		_arraySize		= Max( cubemapArraySizeInCubemaps,	1u ) * 6u;

		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_TEXTURECUBE, true );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetMSAAProperties( uint32 numSamples, uint32 quality ) {
		_sampleDesc.Count	= Max( numSamples, 1u );
		_sampleDesc.Quality	= quality;

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetMIPMapCount( uint32 count ) {
		_mipLevels = count;

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetFormat( ::DXGI_FORMAT format ) {
		_format = format;

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetIsStaticResource() {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_RESOURCE_CLAMP, false );

		SetCPUReadAccessEnabled( false );
		SetCPUWriteAccessEnabled( false );

		_usage = ::D3D11_USAGE_IMMUTABLE;

		return *this;
	}

	// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetIsStreamingResource() {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_RESOURCE_CLAMP, true );

		SetCPUReadAccessEnabled( false );
		SetCPUWriteAccessEnabled( false );

		_usage = ::D3D11_USAGE_DEFAULT;

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetIsStagingResource() {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_RESOURCE_CLAMP, false );

		SetCPUWriteAccessEnabled( true );
		SetCPUReadAccessEnabled( false );

		_usage = ::D3D11_USAGE_STAGING;
		
		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetCPUWriteAccessEnabled( bool isEnabled ) {
		UpdateFlag( _accessFlags, ::D3D11_CPU_ACCESS_WRITE, isEnabled );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetCPUReadAccessEnabled( bool isEnabled ) {
		UpdateFlag( _accessFlags, ::D3D11_CPU_ACCESS_READ, isEnabled );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetIsSharedAcrossDevices( bool isShared ) {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_SHARED, isShared );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetNeedsShaderResourceView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_SHADER_RESOURCE, generateView );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetNeedsRenderTargetView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_RENDER_TARGET, generateView );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetNeedsDepthStencilView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_DEPTH_STENCIL, generateView );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetNeedsUnorderedAccessView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_UNORDERED_ACCESS, generateView );

		return *this;
	}

// ---------------------------------------------------

	TextureBuilder& TextureBuilder::SetDebugName( const char* name ) {
		_debugName = name;

		return *this;
	}

// ---------------------------------------------------

	COMPointer<::ID3D11Resource> TextureBuilder::Compile() {
		struct Texture2DDescriptor : public ::D3D11_TEXTURE2D_DESC {
			ETForceInlineHint Texture2DDescriptor( const TextureBuilder& builder ) {
				Width			= builder._widthInTexels;
				Height			= builder._heightInTexels;
				MipLevels		= builder._mipLevels;
				ArraySize		= builder._arraySize;
				Format			= builder._format;
				SampleDesc		= builder._sampleDesc;
				Usage			= builder._usage;
				BindFlags		= builder._bindFlags;
				CPUAccessFlags	= builder._accessFlags;
				MiscFlags		= builder._miscFlags;
			}
		};

		struct Texture3DDescriptor : public ::D3D11_TEXTURE3D_DESC {
			ETForceInlineHint Texture3DDescriptor( const TextureBuilder& builder ) {
				Width			= builder._widthInTexels;
				Height			= builder._heightInTexels;
				Depth			= builder._depthInTexels;
				MipLevels		= builder._mipLevels;
				Format			= builder._format;
				Usage			= builder._usage;
				BindFlags		= builder._bindFlags;
				CPUAccessFlags	= builder._accessFlags;
				MiscFlags		= builder._miscFlags;
			}
		};

	// ---

		COMPointer<::ID3D11Resource>	result;

		switch( _type ) {
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				const Texture2DDescriptor	descriptor( *this );
				::ID3D11Texture2D*			resource;

				if( SUCCEEDED( _device->CreateTexture2D( &descriptor, nullptr, &resource ) ) ) {
					result = resource;
				}
				
				break;
			}	// case D3D11_RESOURCE_DIMENSION_TEXTURE2D

		// ---

			case ::D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				const Texture3DDescriptor	descriptor( *this );
				::ID3D11Texture3D*			resource;

				if( SUCCEEDED( _device->CreateTexture3D( &descriptor, nullptr, &resource ) ) ) {
					result = resource;
				}
				
				break;
			}	// case D3D11_RESOURCE_DIMENSION_TEXTURE2D

		// ---

			default: {
				ETNoDefaultCaseHint;
			}
		};	// switch( _type )

		if( _debugName && result ) {
			result->SetPrivateData( ::WKPDID_D3DDebugObjectName, static_cast<::UINT>(StringLength( _debugName )), _debugName );
		}

		return move( result );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2