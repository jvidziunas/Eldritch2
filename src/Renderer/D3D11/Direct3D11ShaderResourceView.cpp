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
#include <Renderer/D3D11/Builders/Direct3D11TextureShaderResourceViewBuilder.hpp>
#include <Renderer/D3D11/Builders/Direct3D11TextureBuilder.hpp>
#include <Renderer/D3D11/Direct3D11ShaderResourceView.hpp>
#include <Renderer/Textures/TextureHeader.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Utility/MessagePackReader.hpp>
#include <Utility/ErrorCode.hpp>
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
using namespace ::std;

namespace Eldritch2 {
namespace Renderer {

	Direct3D11ShaderResourceView::Direct3D11ShaderResourceView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), _minLoadedLOD( ::std::numeric_limits<decltype(_minLoadedLOD)>::max() ) {}

// ---------------------------------------------------

	ErrorCode Direct3D11ShaderResourceView::InstantiateFromByteArray( const Range<const char*>& sourceBytes, const COMPointer<::ID3D11Device>& device ) {
		using ShaderResourceViewBuilder	= Direct3D11TextureShaderResourceViewBuilder;
		using TextureBuilder			= Direct3D11TextureBuilder;

	// ---

		TextureHeader	header;

		if( MessagePackReader( sourceBytes )( header ) ) {
			if( auto resource = TextureBuilder( device ).ConfigureFromHeader( header ).SetDebugName( GetName().GetCharacterArray() ).Compile() ) {
				if( auto view = ShaderResourceViewBuilder().Compile( resource ) ) {
					_shaderView = move( view );

					return Error::NONE;
				}
			}
		}

		return Error::INVALID_PARAMETER;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const Direct3D11ShaderResourceView::GetSerializedDataTag() {
		return UTF8L("ShaderResource");
	}

}	// namespace Renderer
}	// namespace Eldritch2