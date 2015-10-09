/*==================================================================*\
  Direct3D11TextureBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
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
		class	D3D11UnorderedAccessViewBuilder;
		class	D3D11ShaderResourceViewBuilder;
		class	D3D11RenderTargetViewBuilder;
		class	D3D11DepthStencilViewBuilder;
		class	Texture;
	}

	class	Allocator;
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
		// Constructs this Direct3D11TextureBuilder instance.
		explicit Direct3D11TextureBuilder( const Renderer::Texture& texture, const ::Eldritch2::UTF8Char* const textureName = nullptr );
		// Constructs this Direct3D11TextureBuilder instance.
		explicit Direct3D11TextureBuilder();

		// Destroys this Direct3D11TextureBuilder instance.
		~Direct3D11TextureBuilder();

	// - BASE TEXTURE LAYOUT CONTROL ---------------------

		Renderer::Direct3D11TextureBuilder&	SetTexture1D( ::Eldritch2::uint32 widthInTexels, ::Eldritch2::uint32 arraySlices = 1u );

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

		// Instructs the builder to allocate extra data for MIP access control.
		Renderer::Direct3D11TextureBuilder&	SetIsStreamingResource();

		// Instructs the builder that the resource will be used as temporary storage for moving data across GPU boundaries.
		Renderer::Direct3D11TextureBuilder&	SetIsStagingResource();

	// - MISCELLANEOUS USAGE CONTROL ---------------------

		// Instructs the builder that the resource will frequently be written to via the CPU.
		Renderer::Direct3D11TextureBuilder&	SetCPUWriteAccessEnabled( bool isEnabled = true );

		// Instructs the builder that the resource will frequently be read from via the CPU.
		Renderer::Direct3D11TextureBuilder&	SetCPUReadAccessEnabled( bool isEnabled = true );

		// Instructs the builder that the resource will be used by multiple Direct3D devices.
		Renderer::Direct3D11TextureBuilder&	SetIsSharedAcrossDevices( bool isShared = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsShaderResourceView( bool generateView = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsRenderTargetView( bool generateView = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsDepthStencilView( bool generateView = true );

		Renderer::Direct3D11TextureBuilder&	SetNeedsUnorderedAccessView( bool generateView = true );

	// - INITIAL DATA -------------------------------------

		// Instructs the builder to populate the texture with data found at the specified address.
		// If this block does not represent the entirety of the texture's contents, a starting
		// MIP level may be passed into the function via the second parameter.
		Renderer::Direct3D11TextureBuilder&	SetInitialData( const void* data, ::Eldritch2::uint32 startMIP = 0u );

	// - DEBUG SPECIAL FEATURES ---------------------------

		// Instructs the builder to attach the specified (null-terminated) C string to the target
		// texture object and any views to it created through this builder instance.
		Renderer::Direct3D11TextureBuilder&	SetDebugName( const char* name );

	// - RESOURCE CONSTRUCTION KICK -----------------------

		::Eldritch2::ErrorCode	Compile( ::Eldritch2::Allocator& temporaryAllocator, ::ID3D11Device* const device );
		::Eldritch2::ErrorCode	Compile( ::Eldritch2::Allocator& temporaryAllocator, const Utility::COMPointer<::ID3D11Device>& device );

	// - GENERATED RESOURCE ACCESS ------------------------

		// Retrieves the current texture object from the builder, or null if the object was (not yet)
		// created successfully.
		const Utility::COMPointer<::ID3D11Resource>&	GetTexture() const;

	// - DATA MEMBERS -------------------------------------

	private:
		::D3D11_RESOURCE_DIMENSION				_type;

		::UINT									_widthInTexels;
		::UINT									_heightInTexels;
		::UINT									_depthInTexels;
		::UINT									_mipLevels;
		::UINT									_arraySize;
		::DXGI_FORMAT							_format;
		::DXGI_SAMPLE_DESC						_sampleDesc;
		::D3D11_USAGE							_usage;
		::UINT									_bindFlags;
		::UINT									_accessFlags;
		::UINT									_miscFlags;

		Utility::COMPointer<::ID3D11Resource>	_resource;

		const void*								_initialData;
		::Eldritch2::uint32						_initialDataStartMIP;
		::Eldritch2::uint32						_initialDataMIPCount;

		const char*								_debugName;

	// - FRIEND CLASS DECLARATIONS ------------------------

		friend class Renderer::D3D11UnorderedAccessViewBuilder;
		friend class Renderer::D3D11ShaderResourceViewBuilder;
		friend class Renderer::D3D11RenderTargetViewBuilder;
		friend class Renderer::D3D11DepthStencilViewBuilder;
	};

}	// namespace Renderer
}	// namespace Eldritch2