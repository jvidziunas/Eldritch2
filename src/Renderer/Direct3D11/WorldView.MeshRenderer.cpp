/*==================================================================*\
  WorldView.MeshRenderer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Renderer/Direct3D11/WorldView.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Packages/ContentLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	const char* const WorldView::MeshRenderer::scriptTypeName = "MeshRenderer";

// ---------------------------------------------------

	WorldView::MeshRenderer::MeshRenderer( Armature& armature, const MeshResourceView& mesh ) : _mesh( mesh ), _armature( armature ) {}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::MeshRenderer::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void WorldView::MeshRenderer::Dispose() {
		GetActiveWorldView()._meshAllocator.Delete( *this );
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2