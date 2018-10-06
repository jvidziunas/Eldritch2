/*==================================================================*\
  NavigationScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	NavigationScene::NavigationScene() ETNoexceptHint {}

	// ---------------------------------------------------

	void NavigationScene::Simulate(MicrosecondTime duration) {
		dtCrowd::update(AsSeconds(duration), /*debug =*/nullptr);
	}

	// ---------------------------------------------------

	ErrorCode NavigationScene::BindResources(const dtNavMeshParams& meshParameters, int maxAgents, float32 agentRadius) {
		ET_ABORT_UNLESS(dtNavMesh::init(ETAddressOf(meshParameters)) == DT_SUCCESS ? Error::None : Error::Unspecified);
		ET_ABORT_UNLESS(dtCrowd::init(maxAgents, agentRadius, this) ? Error::None : Error::Unspecified);

		return Error::None;
	}

	// ---------------------------------------------------

	void NavigationScene::FreeResources() {}

}}} // namespace Eldritch2::Navigation::Recast
