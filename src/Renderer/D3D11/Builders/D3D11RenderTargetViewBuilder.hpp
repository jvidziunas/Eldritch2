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
#include <Renderer/D3D11/Builders/Direct3D11TextureBuilder.hpp>
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
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
		//!	Constructs this @ref D3D11RenderTargetViewBuilder instance.
		D3D11RenderTargetViewBuilder();

		//!	Destroys this @ref D3D11RenderTargetViewBuilder instance.
		~D3D11RenderTargetViewBuilder() = default;

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

		Utility::COMPointer<::ID3D11RenderTargetView>	Compile( const Utility::COMPointer<::ID3D11Resource>& targetResource );
		Utility::COMPointer<::ID3D11RenderTargetView>	Compile( ::ID3D11Resource* const targetResource );
		Utility::COMPointer<::ID3D11RenderTargetView>	Compile( const Utility::COMPointer<::ID3D11Texture2D>& targetResource );
		Utility::COMPointer<::ID3D11RenderTargetView>	Compile( ::ID3D11Texture2D* const targetResource );
		Utility::COMPointer<::ID3D11RenderTargetView>	Compile( const Utility::COMPointer<::ID3D11Texture3D>& targetResource );
		Utility::COMPointer<::ID3D11RenderTargetView>	Compile( ::ID3D11Texture3D* const targetResource );

	// - DATA MEMBERS -------------------------------------

	private:
		::DXGI_FORMAT	_formatOverride;
		::UINT			_initialSlice;
		::UINT			_sizeInSlices;
		::UINT			_mipIndex;

		const char*		_debugName;
	};

}	// namespace Renderer
}	// namespace Eldritch2