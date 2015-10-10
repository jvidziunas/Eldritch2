/*==================================================================*\
  Direct3D11ShaderResourceView.cpp
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
#include <Renderer/D3D11/Direct3D11ShaderResourceView.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "D3D11.lib" )
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {

	Direct3D11ShaderResourceView::Direct3D11ShaderResourceView( COMPointer<::ID3D11ShaderResourceView>&& shaderView, const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ),
																																											  _shaderView( ::std::move( shaderView ) ),
																																											  _minLoadedLOD( ::std::numeric_limits<decltype(_minLoadedLOD)>::max() ) {
		ETRuntimeAssert( _shaderView );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const Direct3D11ShaderResourceView::GetSerializedDataTag() {
		return UTF8L("ShaderResource");
	}

}	// namespace Renderer
}	// namespace Eldritch2