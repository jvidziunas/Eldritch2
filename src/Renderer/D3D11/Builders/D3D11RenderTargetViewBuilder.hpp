/*==================================================================*\
  D3D11RenderTargetViewBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
#include <Utility/MPL/Noncopyable.hpp>
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
		class	Direct3D11TextureBuilder;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class D3D11RenderTargetViewBuilder : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this D3D11RenderTargetViewBuilder instance.
		explicit D3D11RenderTargetViewBuilder( Renderer::Direct3D11TextureBuilder& textureBuilder );

		// Destroys this D3D11RenderTargetViewBuilder instance.
		~D3D11RenderTargetViewBuilder();

	// - MULTISAMPLING CONTROL ---------------------------

		Renderer::D3D11RenderTargetViewBuilder&	SetDesiredSlices( ::Eldritch2::uint32 initialArraySlice, ::Eldritch2::uint32 arraySizeInSlices );

		Renderer::D3D11RenderTargetViewBuilder&	SetDesiredSlice( ::Eldritch2::uint32 arraySlice );

	// - TARGET TEXTURE MIPMAP CONTROL -------------------

		Renderer::D3D11RenderTargetViewBuilder&	SetMIPMapIndex( ::Eldritch2::uint32 index );

	// - TEXTURE FORMAT CONTROL --------------------------

		Renderer::D3D11RenderTargetViewBuilder&	OverrideFormat( ::DXGI_FORMAT format );

	// - DEBUG SPECIAL FEATURES ---------------------------

		// Instructs the builder to attach the specified (null-terminated) C string to the target
		// texture object and any views to it created through this builder instance.
		Renderer::D3D11RenderTargetViewBuilder&	SetDebugName( const char* name );

	// - RESOURCE CONSTRUCTION KICK -----------------------

		::Eldritch2::ErrorCode	Compile( ::ID3D11Device* const device );
		::Eldritch2::ErrorCode	Compile( const Utility::COMPointer<::ID3D11Device>& device );

	// - GENERATED RESOURCE ACCESS ------------------------

		const Utility::COMPointer<::ID3D11RenderTargetView>&	GetView() const;

	// - DATA MEMBERS -------------------------------------

	private:
		Utility::COMPointer<::ID3D11RenderTargetView>	_view;
		Renderer::Direct3D11TextureBuilder&				_textureBuilder;

		::DXGI_FORMAT									_format;
		::UINT											_initialSlice;
		::UINT											_sizeInSlices;
		::UINT											_mipIndex;

		const char*										_debugName;
	};

}	// namespace Renderer
}	// namespace Eldritch2