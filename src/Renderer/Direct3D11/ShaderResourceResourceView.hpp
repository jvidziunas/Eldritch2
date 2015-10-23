/*==================================================================*\
  ShaderResourceResourceView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceView.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class ShaderResourceResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ShaderResourceResourceView instance.
		ShaderResourceResourceView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ShaderResourceResourceView instance.
		~ShaderResourceResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	InstantiateFromByteArray( const ::Eldritch2::Range<const char*>& sourceBytes, const Utility::COMPointer<::ID3D11Device>& device );

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::COMPointer<::ID3D11ShaderResourceView>	_shaderView;
		::Eldritch2::uint32								_minLoadedLOD;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2