/*==================================================================*\
  Direct3D11ShaderResourceView.hpp
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
#include <Packages/ResourceView.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11ShaderResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Direct3D11ShaderResourceView instance.
		Direct3D11ShaderResourceView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref Direct3D11ShaderResourceView instance.
		~Direct3D11ShaderResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	InstantiateFromByteArray( const ::Eldritch2::Range<const char*>& sourceBytes, const Utility::COMPointer<::ID3D11Device>& device );

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::COMPointer<::ID3D11ShaderResourceView>	_shaderView;
		::Eldritch2::uint32								_minLoadedLOD;
	};

}	// namespace Renderer
}	// namespace Eldritch2