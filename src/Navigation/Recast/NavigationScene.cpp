/*==================================================================*\
  NavigationScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	NavigationScene::NavigationScene() ETNoexceptHint {}

	// ---------------------------------------------------

	void NavigationScene::Simulate(MicrosecondTime duration) {
		dtCrowd::update(AsMilliseconds(duration) / /*milliseconds per second*/1000.0f, /*debug =*/nullptr);
	}

	// ---------------------------------------------------

	Result NavigationScene::BindResources(int maxAgents, float32 agentRadius, const dtNavMeshParams& meshParameters) {
		ET_ABORT_UNLESS(dtNavMesh::init(ETAddressOf(meshParameters)) == DT_SUCCESS ? Result::Success : Result::Unspecified);
		ET_ABORT_UNLESS(dtCrowd::init(maxAgents, agentRadius, this) ? Result::Success : Result::Unspecified);

		return Result::Success;
	}

	// ---------------------------------------------------

	void NavigationScene::FreeResources() ETNoexceptHint {}

}}} // namespace Eldritch2::Navigation::Recast
