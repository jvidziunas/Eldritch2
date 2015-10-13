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
#include <Renderer/D3D11/Builders/Direct3D11TextureBuilder.hpp>
#include <Renderer/D3D11/D3D11FormatHelpers.hpp>
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Renderer/Textures/Texture.hpp>
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

namespace {

	static ETForceInlineHint ETNoAliasHint void UpdateFlag( ::UINT& flags, const ::UINT flag, bool enabled ) {
		const ::UINT newValue( ( flags | static_cast<::UINT>( enabled ? flag : 0u ) ) & ~( enabled ? flag : 0u ) );
		flags = newValue;
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Renderer {

	Direct3D11TextureBuilder::Direct3D11TextureBuilder( const Texture& texture, const UTF8Char* const textureName ) {
		switch( texture.GetType() ) {
			case TextureType::TEXTURE_1D: {
				SetTexture1D( texture.GetWidthInTexels() );
				break;
			}
			case TextureType::TEXTURE_2D: {
				SetTexture2D( texture.GetWidthInTexels(), texture.GetHeightInTexels(), texture.GetArraySize() );
				break;
			}
			case TextureType::TEXTURE_3D: {
				SetTexture3D( texture.GetWidthInTexels(), texture.GetHeightInTexels(), texture.GetDepthInTexels() );
				break;
			}
			case TextureType::CUBEMAP: {
				SetCubemap( texture.GetWidthInTexels(), texture.GetHeightInTexels(), texture.GetArraySize() );
				break;
			}
		};

		SetMIPMapCount( texture.GetTotalMIPCount() ).SetFormat( BuildDXGIFormat( texture.GetSamplingDescriptor() ) );
		SetInitialData( texture.GetData(), texture.GetPresentMIPOffset() ).SetNeedsShaderResourceView( true );

		// Disk-loaded textures are never MSAA.
		_sampleDesc.Count	= 1u;
		_sampleDesc.Quality	= 0u;

		if( texture.ContainsCompleteMIPChain() ) {
			SetIsStaticResource();
		} else {
			SetIsStreamingResource();
		}

		SetDebugName( textureName );
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder::Direct3D11TextureBuilder() : _widthInTexels( 1u ),
														   _heightInTexels( 1u ),
														   _depthInTexels( 1u ),
														   _mipLevels( 1u ),
														   _arraySize( 1u ),
														   _format( ::DXGI_FORMAT_UNKNOWN ),
														   _usage( ::D3D11_USAGE_DEFAULT ),
														   _bindFlags( 0u ),
														   _accessFlags( 0u ),
														   _miscFlags( 0u ),
														   _initialData( nullptr ),
														   _initialDataStartMIP( 0u ),
														   _debugName( nullptr ) {
		_sampleDesc.Count	= 1u;
		_sampleDesc.Quality	= 0u;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder::~Direct3D11TextureBuilder() {}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetTexture1D( uint32 widthInTexels, uint32 arraySlices ) {
		_type	= ::D3D11_RESOURCE_DIMENSION_TEXTURE1D;

		_widthInTexels	= Max( widthInTexels,	1u );
		_heightInTexels	= 1u;
		_arraySize		= Max( arraySlices,		1u );

		// We're not a cubemap. Clear the flag in case it was set previously.
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_TEXTURECUBE, false );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetTexture2D( uint32 widthInTexels, uint32 heightInTexels, uint32 arraySlices ) {
		_type			= ::D3D11_RESOURCE_DIMENSION_TEXTURE2D;

		_widthInTexels	= Max( widthInTexels,	1u );
		_heightInTexels	= Max( heightInTexels,	1u );
		_arraySize		= Max( arraySlices,		1u );

		// We're not a cubemap. Clear the flag in case it was set previously.
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_TEXTURECUBE, false );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetTexture3D( uint32 widthInTexels, uint32 heightInTexels, uint32 depthInTexels ) {
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

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetCubemap( uint32 widthInTexels, uint32 heightInTexels, uint32 cubemapArraySizeInCubemaps ) {
		_type			= ::D3D11_RESOURCE_DIMENSION_TEXTURE2D;

		_widthInTexels	= Max( widthInTexels,				1u );
		_heightInTexels	= Max( heightInTexels,				1u );
		_arraySize		= Max( cubemapArraySizeInCubemaps,	1u ) * 6u;

		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_TEXTURECUBE, true );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetMSAAProperties( uint32 numSamples, uint32 quality ) {
		_sampleDesc.Count	= Max( numSamples, 1u );
		_sampleDesc.Quality	= quality;

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetMIPMapCount( uint32 count ) {
		_mipLevels = count;

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetFormat( ::DXGI_FORMAT format ) {
		_format = format;

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetIsStaticResource() {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_RESOURCE_CLAMP, false );

		SetCPUReadAccessEnabled( false );
		SetCPUWriteAccessEnabled( false );

		_usage = ::D3D11_USAGE_IMMUTABLE;

		return *this;
	}

	// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetIsStreamingResource() {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_RESOURCE_CLAMP, true );

		SetCPUReadAccessEnabled( false );
		SetCPUWriteAccessEnabled( false );

		_usage = ::D3D11_USAGE_DEFAULT;

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetIsStagingResource() {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_RESOURCE_CLAMP, false );

		SetCPUWriteAccessEnabled( true );
		SetCPUReadAccessEnabled( false );

		_usage = ::D3D11_USAGE_STAGING;
		
		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetCPUWriteAccessEnabled( bool isEnabled ) {
		UpdateFlag( _accessFlags, ::D3D11_CPU_ACCESS_WRITE, isEnabled );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetCPUReadAccessEnabled( bool isEnabled ) {
		UpdateFlag( _accessFlags, ::D3D11_CPU_ACCESS_READ, isEnabled );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetIsSharedAcrossDevices( bool isShared ) {
		UpdateFlag( _miscFlags, ::D3D11_RESOURCE_MISC_SHARED, isShared );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetNeedsShaderResourceView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_SHADER_RESOURCE, generateView );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetNeedsRenderTargetView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_RENDER_TARGET, generateView );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetNeedsDepthStencilView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_DEPTH_STENCIL, generateView );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetNeedsUnorderedAccessView( bool generateView ) {
		UpdateFlag( _bindFlags, ::D3D11_BIND_UNORDERED_ACCESS, generateView );

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetInitialData( const void* data, uint32 startMIP ) {
		_initialData			= data;
		_initialDataStartMIP	= startMIP;

		return *this;
	}

// ---------------------------------------------------

	Direct3D11TextureBuilder& Direct3D11TextureBuilder::SetDebugName( const char* name ) {
		_debugName = name;

		return *this;
	}

// ---------------------------------------------------

	ErrorCode Direct3D11TextureBuilder::Compile( Allocator& temporaryAllocator, const COMPointer<::ID3D11Device>& device ) {
		return Compile( temporaryAllocator, device.GetUnadornedPointer() );
	}

// ---------------------------------------------------

	ErrorCode Direct3D11TextureBuilder::Compile( Allocator& temporaryAllocator, ::ID3D11Device* const device ) {
		struct Texture1DDescriptor : public ::D3D11_TEXTURE1D_DESC {
			ETForceInlineHint Texture1DDescriptor( const Direct3D11TextureBuilder& builder ) {
				Width			= builder._widthInTexels;
				MipLevels		= builder._mipLevels;
				ArraySize		= builder._arraySize;
				Format			= builder._format;
				Usage			= builder._usage;
				BindFlags		= builder._bindFlags;
				CPUAccessFlags	= builder._accessFlags;
				MiscFlags		= builder._miscFlags;
			}
		};

		struct Texture2DDescriptor : public ::D3D11_TEXTURE2D_DESC {
			ETForceInlineHint Texture2DDescriptor( const Direct3D11TextureBuilder& builder ) {
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
			ETForceInlineHint Texture3DDescriptor( const Direct3D11TextureBuilder& builder ) {
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

		::D3D11_SUBRESOURCE_DATA*	resourceInitialData( nullptr );
		::HRESULT					result;

		if( _initialData ) {
			::D3D11_SUBRESOURCE_DATA* const	ETRestrictPtrHint	initializationData( AllocateTemporaryArray<::D3D11_SUBRESOURCE_DATA>( temporaryAllocator, _mipLevels * _arraySize ) );
			const ::UINT										initialDataStartMIP( _initialDataStartMIP );
			const ::UINT										mipLevelsInResource( _mipLevels );
			const char*											initialDataSourcePointer( static_cast<const char*>( _initialData ) );

			for( ::UINT arraySlice( 0u ), endSlice( _arraySize ); arraySlice < endSlice; ++arraySlice ) {
				for( ::UINT mipLevel( 0u ); mipLevel < initialDataStartMIP; ++mipLevel ) {
					::D3D11_SUBRESOURCE_DATA&	subresourceData( initializationData[::D3D11CalcSubresource( mipLevel, arraySlice, mipLevelsInResource )] );
					// Zero out any MIP levels the caller has specified are not present yet.
					subresourceData.pSysMem				= nullptr;
					subresourceData.SysMemPitch			= 0u;
					subresourceData.SysMemSlicePitch	= 0u;
				}

				::UINT	currentWidth( Max( _widthInTexels >> initialDataStartMIP, 1u ) );
				::UINT	currentHeight( Max( _heightInTexels >> initialDataStartMIP, 1u ) );
				::UINT	currentDepth( Max( _depthInTexels >> initialDataStartMIP, 1u ) );

				for( ::UINT mipLevel( initialDataStartMIP ); mipLevel < mipLevelsInResource; ++mipLevel ) {
					::D3D11_SUBRESOURCE_DATA&	subresourceData( initializationData[::D3D11CalcSubresource( mipLevel, arraySlice, mipLevelsInResource )] );
					uint32						sliceTotalBytes;
					uint32						rowBytes;
					uint32						rowCount;

					GetSurfaceInfo( currentWidth, currentHeight, _format, sliceTotalBytes, rowBytes, rowCount );

					subresourceData.pSysMem				= initialDataSourcePointer;
					subresourceData.SysMemPitch			= rowBytes;
					subresourceData.SysMemSlicePitch	= sliceTotalBytes;

					initialDataSourcePointer += sliceTotalBytes * currentDepth;

					currentWidth	= Max( currentWidth / 2u,	1u );
					currentHeight	= Max( currentHeight / 2u,	1u );
					currentDepth	= Max( currentDepth / 2u,	1u );
				}
			}

			resourceInitialData = initializationData;
		}

		switch( _type ) {
			case ::D3D11_RESOURCE_DIMENSION_TEXTURE1D: {
				COMPointer<::ID3D11Texture1D>	resource;
				const Texture1DDescriptor		descriptor( *this );

				result = device->CreateTexture1D( &descriptor, resourceInitialData, resource.GetInterfacePointer() );
				_resource.Acquire( resource.GetUnadornedPointer() );
				break;
			}	// case D3D11_RESOURCE_DIMENSION_TEXTURE1D

		// ---

			case ::D3D11_RESOURCE_DIMENSION_TEXTURE2D: {
				COMPointer<::ID3D11Texture2D>	resource;
				const Texture2DDescriptor		descriptor( *this );

				result = device->CreateTexture2D( &descriptor, resourceInitialData, resource.GetInterfacePointer() );
				_resource.Acquire( resource.GetUnadornedPointer() );
				break;
			}	// case D3D11_RESOURCE_DIMENSION_TEXTURE2D

		// ---

			case ::D3D11_RESOURCE_DIMENSION_TEXTURE3D: {
				COMPointer<::ID3D11Texture3D>	resource;
				const Texture3DDescriptor		descriptor( *this );

				result = device->CreateTexture3D( &descriptor, resourceInitialData, resource.GetInterfacePointer() );
				_resource.Acquire( resource.GetUnadornedPointer() );
				break;
			}	// case D3D11_RESOURCE_DIMENSION_TEXTURE2D

		// ---

			default: {
				result = E_FAIL;
				break;
			}
		};

		if( _debugName && _resource ) {
			_resource->SetPrivateData( ::WKPDID_D3DDebugObjectName, static_cast<::UINT>( StringLength( _debugName ) ), _debugName );
		}

		if( resourceInitialData ) {
			DeleteTemporaryArray( temporaryAllocator, resourceInitialData );
		}

		return SUCCEEDED( result ) ? Error::NONE : Error::INVALID_OBJECT_STATE;
	}

// ---------------------------------------------------

	const COMPointer<::ID3D11Resource>& Direct3D11TextureBuilder::GetTexture() const {
		return _resource;
	}

}	// namespace Renderer
}	// namespace Eldritch2