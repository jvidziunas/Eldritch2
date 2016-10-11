/*==================================================================*\
  SceneRenderer.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/ScriptComponents/SceneCamera.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	ETInlineHint void SceneRenderer::AttachCamera( const ScriptComponents::SceneCamera& camera ) {
	/*	This function can be called at any point during the script update; we need to make sure
	 *	there is no stompage of the world camera collection. */
		Platform::ScopedWriteLock	_( _cameraMutex );

	//	The collection is ordered to help with efficiency/de-duplication.
		_cameras.Insert( &camera );
	}

// ---------------------------------------------------

	ETInlineHint void SceneRenderer::DetachCamera( const ScriptComponents::SceneCamera& camera ) {
	/*	This function can be called at any point during the script update; we need to make sure
	 *	there is no stompage of the world camera collection. */
		Platform::ScopedWriteLock	_( _cameraMutex );

	//	The collection is ordered to help with efficiency.
		_cameras.Erase( &camera );
	}

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

