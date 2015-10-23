/*==================================================================*\
  TextureBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/FlatOrderedMap.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		struct	TextureHeader;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class TextureBuilder : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TextureBuilder instance.
		TextureBuilder( const Utility::COMPointer<::ID3D11Device>& device );

		//!	Destroys this @ref TextureBuilder instance.
		~TextureBuilder() = default;

	// ---------------------------------------------------

		TextureBuilder&	ConfigureFromHeader( const Renderer::TextureHeader& header );

	// - BASE TEXTURE LAYOUT CONTROL ---------------------

		TextureBuilder&	SetTexture2D( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 heightInTexels, ::Eldritch2::uint32 arraySlices = 1u );

		TextureBuilder&	SetTexture3D( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 heightInTexels, ::Eldritch2::uint32 depthInTexels );

		TextureBuilder&	SetCubemap( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 heightInTexels, ::Eldritch2::uint32 cubemapArraySizeInCubemaps = 1u );

	// - MULTISAMPLING CONTROL ---------------------------

		TextureBuilder&	SetMSAAProperties( ::Eldritch2::uint32 numSamples, ::Eldritch2::uint32 quality );

	// - TEXTURE MIPMAP LIMIT CONTROL --------------------

		TextureBuilder&	SetMIPMapCount( ::Eldritch2::uint32 count );

	// - TEXTURE FORMAT CONTROL --------------------------

		TextureBuilder&	SetFormat( ::DXGI_FORMAT format );

	// - TEXTURE SPECIAL USAGE CONTROL -------------------

		TextureBuilder&	SetIsStaticResource();

		//!	Instructs the builder to allocate extra data for MIP access control.
		TextureBuilder&	SetIsStreamingResource();

		//!	Instructs the builder that the resource will be used as temporary storage for moving data across GPU boundaries.
		TextureBuilder&	SetIsStagingResource();

	// - MISCELLANEOUS USAGE CONTROL ---------------------

		//! Instructs the builder that the resource will frequently be written to via the CPU.
		TextureBuilder&	SetCPUWriteAccessEnabled( bool isEnabled = true );

		//! Instructs the builder that the resource will frequently be read from via the CPU.
		TextureBuilder&	SetCPUReadAccessEnabled( bool isEnabled = true );

		//! Instructs the builder that the resource will be used by multiple Direct3D devices.
		TextureBuilder&	SetIsSharedAcrossDevices( bool isShared = true );

		TextureBuilder&	SetNeedsShaderResourceView( bool generateView = true );

		TextureBuilder&	SetNeedsRenderTargetView( bool generateView = true );

		TextureBuilder&	SetNeedsDepthStencilView( bool generateView = true );

		TextureBuilder&	SetNeedsUnorderedAccessView( bool generateView = true );

	// - DEBUG SPECIAL FEATURES ---------------------------

		//!	Instructs the builder to attach the specified (null-terminated) C string to the target texture object and any views to it created through this builder instance.
		TextureBuilder&	SetDebugName( const char* name );

	// - RESOURCE CONSTRUCTION KICK -----------------------

		Utility::COMPointer<::ID3D11Resource>	Compile();

	// - DATA MEMBERS -------------------------------------

	private:
		Utility::COMPointer<::ID3D11Device>	_device;

		::D3D11_RESOURCE_DIMENSION			_type;

		::UINT								_widthInTexels;
		::UINT								_heightInTexels;
		::UINT								_depthInTexels;
		::UINT								_mipLevels;
		::UINT								_arraySize;
		::DXGI_FORMAT						_format;
		::DXGI_SAMPLE_DESC					_sampleDesc;
		::D3D11_USAGE						_usage;
		::UINT								_bindFlags;
		::UINT								_accessFlags;
		::UINT								_miscFlags;

		const char*							_debugName;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2