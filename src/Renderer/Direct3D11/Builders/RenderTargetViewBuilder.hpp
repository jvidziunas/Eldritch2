/*==================================================================*\
  RenderTargetViewBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/ForwardDeclarations.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		template <class Interface>
		class	COMPointer;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class RenderTargetViewBuilder {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderTargetViewBuilder instance.
		RenderTargetViewBuilder();

		//!	Destroys this @ref RenderTargetViewBuilder instance.
		~RenderTargetViewBuilder() = default;

	// ---------------------------------------------------

		RenderTargetViewBuilder&	SetDesiredSlices( ::Eldritch2::uint32 initialArraySlice, ::Eldritch2::uint32 arraySizeInSlices );

		RenderTargetViewBuilder&	SetDesiredSlice( ::Eldritch2::uint32 arraySlice );

	// - TARGET TEXTURE MIPMAP CONTROL -------------------

		RenderTargetViewBuilder&	SetMIPMapIndex( ::Eldritch2::uint32 index );

	// - TEXTURE FORMAT CONTROL --------------------------

		RenderTargetViewBuilder&	OverrideFormat( ::DXGI_FORMAT format );

	// - DEBUG SPECIAL FEATURES ---------------------------

		// Instructs the builder to attach the specified (null-terminated) C string to the target
		// texture object and any views to it created through this builder instance.
		RenderTargetViewBuilder&	SetDebugName( const char* name );

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

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2