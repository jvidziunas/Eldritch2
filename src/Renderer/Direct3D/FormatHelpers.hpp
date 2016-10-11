/*==================================================================*\
  FormatHelpers.hpp
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
namespace Direct3D {

	ETPureFunctionHint Eldritch2::uint32	BitsPerPixel( DXGI_FORMAT fmt );

// ---------------------------------------------------

	ETPureFunctionHint void	GetSurfaceInfo( Eldritch2::uint32 width, Eldritch2::uint32 height, DXGI_FORMAT fmt, Eldritch2::uint32& numBytes, Eldritch2::uint32& rowBytes, Eldritch2::uint32& numRows );

// ---------------------------------------------------

	ETPureFunctionHint DXGI_FORMAT	BuildDxgiFormat( const Renderer::GpuLayout layout, const Renderer::GpuStorageFormat format, const Renderer::FilteringFlags flags );

// ---------------------------------------------------

	ETPureFunctionHint DXGI_FORMAT	ResourceToTypelessFormat( DXGI_FORMAT resourceFormat );

	ETPureFunctionHint DXGI_FORMAT	ResourceToShaderFormat( DXGI_FORMAT resourceFormat );

	ETPureFunctionHint DXGI_FORMAT	ResourceToRenderTargetFormat( DXGI_FORMAT resourceFormat );

	ETPureFunctionHint DXGI_FORMAT	ResourceToDepthStencilFormat( DXGI_FORMAT resourceFormat );

}	// namespace Direct3D
}	// namespace Renderer
}	// namespace Eldritch2