/*==================================================================*\
  WorldView.SceneCamera.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Renderer/Direct3D11/SwapChain.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	const char* const WorldView::SceneCamera::scriptTypeName = "SceneCamera";

// ---------------------------------------------------

	WorldView::SceneCamera::SceneCamera( SwapChain& swapChain, WorldView& owningView ) : _swapChain( swapChain ), _cachedUsages( { owningView.GetWorldAllocator(), UTF8L("Scene Camera Component Cached Material Usage Allocator") } ) {
		owningView._attachedCameras.PushFront( *this );
		_cachedUsages.Reserve( 64u );
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::SceneCamera::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void WorldView::SceneCamera::Dispose() {
		auto&	worldView( GetActiveWorldView() );
		auto&	attachedCameras( worldView._attachedCameras );

		attachedCameras.Erase( attachedCameras.IteratorTo( *this ) );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2