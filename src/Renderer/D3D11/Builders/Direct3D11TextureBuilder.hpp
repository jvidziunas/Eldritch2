/*==================================================================*\
  Direct3D11TextureBuilder.hpp
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
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11TextureBuilder : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Direct3D11TextureBuilder instance.
		Direct3D11TextureBuilder( const Utility::COMPointer<::ID3D11Device>& device );

		//!	Destroys this @ref Direct3D11TextureBuilder instance.
		~Direct3D11TextureBuilder() = default;

	// - BASE TEXTURE LAYOUT CONTROL ---------------------

		Renderer::Direct3D11TextureBuilder&	SetTexture2D( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 heightInTexels, ::Eldritch2::uint32 arraySlices = 1u );

		Renderer::Direct3D11TextureBuilder&	SetTexture3D( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 heightInTexels, ::Eldritch2::uint32 depthInTexels );

		Renderer::Direct3D11TextureBuilder&	SetCubemap( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 heightInTexels, ::Eldritch2::uint32 cubemapArraySizeInCubemaps = 1u );

	// - MULTISAMPLING CONTROL ---------------------------

		Renderer::Direct3D11TextureBuilder&	SetMSAAProperties( ::Eldritch2::uint32 numSamples, ::Eldritch2::uint32 quality );

	// - TEXTURE MIPMAP LIMIT CONTROL --------------------

		Renderer::Direct3D11TextureBuilder&	SetMIPMapCount( ::Eldritch2::uint32 count );

	// - TEXTURE FORMAT CONTROL --------------------------

		Renderer::Direct3D11TextureBuilder&	SetFormat( ::DXGI_FORMAT format );

	// - TEXTURE SPECIAL USAGE CONTROL -------------------

		Renderer::Direct3D11TextureBuilder&	SetIsStaticResource();

		//!	Instructs the builder to allocate extra data for MIP access control.
		Renderer::Direct3D11TextureBuilder&	SetIsStreamingResource();

		//!	Instructs the builder that the resource will be used as temporary storage for moving data across GPU boundaries.
		Renderer::Direct3D11TextureBuilder&	SetIsStagingResource();

	// - MISCELLANEOUS USAGE CONTROL ---------------------

		//! Instructs the builder that the resource will frequently be written to via the CPU.
		Renderer::Direct3D11TextureBuilder&	SetCPUWriteAccessEnabled( bool isEnabled = true );

		//! Instructs the builder that the resource will frequently be read from via the CPU.
		Renderer::Direct3D11TextureBuilder&	SetCPUReadAccessEnabled( bool isEnabled = true );

		//! Instructs the builder that the resource will be used by multiple Direct3D devices.
		Renderer::Direct3D11TextureBuilder&	SetIsSharedAcrossDevices( bool isShared = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsShaderResourceView( bool generateView = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsRenderTargetView( bool generateView = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsDepthStencilView( bool generateView = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsUnorderedAccessView( bool generateView = true );

	// - DEBUG SPECIAL FEATURES ---------------------------

		//!	Instructs the builder to attach the specified (null-terminated) C string to the target texture object and any views to it created through this builder instance.
		Renderer::Direct3D11TextureBuilder&	SetDebugName( const char* name );

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

}	// namespace Renderer
}	// namespace Eldritch2