/*==================================================================*\
  ShaderResourceResourceView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/Builders/TextureShaderResourceViewBuilder.hpp>
#include <Renderer/Direct3D11/Builders/TextureBuilder.hpp>
#include <Renderer/Direct3D11/ShaderResourceResourceView.hpp>
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
namespace Direct3D11 {

	ShaderResourceResourceView::ShaderResourceResourceView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), _minLoadedLOD( ::std::numeric_limits<decltype(_minLoadedLOD)>::max() ) {}

// ---------------------------------------------------

	ErrorCode ShaderResourceResourceView::InstantiateFromByteArray( const Range<const char*>& sourceBytes, const COMPointer<::ID3D11Device>& device ) {
		using TextureBuilder	= Direct3D11::TextureBuilder;
		using ViewBuilder		= Direct3D11::TextureShaderResourceViewBuilder;

	// ---

		TextureHeader	header;

		if( MessagePackReader( sourceBytes )( header ) ) {
			if( auto resource = TextureBuilder( device ).ConfigureFromHeader( header ).SetDebugName( GetName().GetCharacterArray() ).Compile() ) {
				if( auto view = ViewBuilder().Compile( resource ) ) {
					_shaderView = move( view );

					return Error::NONE;
				}
			}
		}

		return Error::INVALID_PARAMETER;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ShaderResourceResourceView::GetSerializedDataTag() {
		return UTF8L("ShaderResource");
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2