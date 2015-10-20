/*==================================================================*\
  D3D11FormatHelpers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/GPUFormats.hpp>
//------------------------------------------------------------------//

enum	DXGI_FORMAT;

namespace Eldritch2 {
namespace Renderer {

	namespace Renderer	= ::Eldritch2::Renderer;

// ---------------------------------------------------

	ETNoAliasHint ::Eldritch2::uint32	BitsPerPixel( ::DXGI_FORMAT fmt );

// ---------------------------------------------------

	ETNoAliasHint void	GetSurfaceInfo( ::Eldritch2::uint32 width, ::Eldritch2::uint32 height, ::DXGI_FORMAT fmt, ::Eldritch2::uint32& numBytes, ::Eldritch2::uint32& rowBytes, Eldritch2::uint32& numRows );

// ---------------------------------------------------

	ETNoAliasHint::DXGI_FORMAT	BuildDXGIFormat( const Renderer::GPULayout layout, const Renderer::GPUStorageFormat format, const Renderer::FilteringFlags flags );

// ---------------------------------------------------

	ETNoAliasHint ::DXGI_FORMAT	ResourceToTypelessFormat( ::DXGI_FORMAT resourceFormat );

	ETNoAliasHint ::DXGI_FORMAT	ResourceToSRVFormat( ::DXGI_FORMAT resourceFormat );

	ETNoAliasHint ::DXGI_FORMAT	ResourceToRTVFormat( ::DXGI_FORMAT resourceFormat );

	ETNoAliasHint ::DXGI_FORMAT	ResourceToDSVFormat( ::DXGI_FORMAT resourceFormat );

}	// namespace Renderer
}	// namespace Eldritch2