/*==================================================================*\
  Direct3D11RendererService.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {

	ETInlineHint const Utility::COMPointer<::ID3D11Device>&	Direct3D11RendererService::GetDevice() const {
		return _device;
	}

}	// namespace Renderer
}	// namespace Eldritch2


