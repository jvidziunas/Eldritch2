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
#include <Graphics/GpuFormats.hpp>
//------------------------------------------------------------------//

enum	DXGI_FORMAT;

namespace Eldritch2 {
	namespace Graphics {
		namespace Direct3D {

			ETPureFunctionHint uint32		GetBitsPerPixel(DXGI_FORMAT format);

			ETPureFunctionHint void			GetSurfaceInfo(uint32 width, uint32 height, DXGI_FORMAT format, uint32& numBytes, uint32& rowBytes, uint32& numRows);

			ETPureFunctionHint DXGI_FORMAT	GetDxgiFormat(GpuBitLayout layout, GpuStorageFormat format, GpuFilteringFlags flags);

			ETPureFunctionHint DXGI_FORMAT	GetTypelessFormat(DXGI_FORMAT format);

			ETPureFunctionHint DXGI_FORMAT	GetShaderFormat(DXGI_FORMAT format);

			ETPureFunctionHint DXGI_FORMAT	GetRenderTargetFormat(DXGI_FORMAT format);

			ETPureFunctionHint DXGI_FORMAT	GetDepthStencilFormat(DXGI_FORMAT format);

		}	// namespace Direct3D
	}	// namespace Graphics
}	// namespace Eldritch2