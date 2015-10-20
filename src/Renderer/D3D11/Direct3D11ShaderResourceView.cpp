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
#include <Renderer/Textures/ShaderResourceHeader.hpp>
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

namespace Eldritch2 {
namespace Renderer {

	Direct3D11ShaderResourceView::Direct3D11ShaderResourceView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), _minLoadedLOD( ::std::numeric_limits<decltype(_minLoadedLOD)>::max() ) {}

// ---------------------------------------------------

	ErrorCode Direct3D11ShaderResourceView::InstantiateFromByteArray( const Range<const char*>& sourceBytes, const COMPointer<::ID3D11Device>& device ) {
		ShaderResourceHeader	header;

		if( MessagePackReader( sourceBytes )( header ) ) {
			Direct3D11TextureBuilder	builder( device );
			
			// Set shared state
			builder.SetDebugName( GetName().GetCharacterArray() ).SetNeedsShaderResourceView();

			header.ConfigureResourceBuilder( builder );

			auto	resource = builder.Compile();

			return Error::NONE;
		}

		return Error::INVALID_PARAMETER;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const Direct3D11ShaderResourceView::GetSerializedDataTag() {
		return UTF8L("ShaderResource");
	}

}	// namespace Renderer
}	// namespace Eldritch2