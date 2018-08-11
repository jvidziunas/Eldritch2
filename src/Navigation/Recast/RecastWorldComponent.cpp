/*==================================================================*\
  RecastWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/RecastWorldComponent.hpp>
#include <Navigation/Recast/NavigationScene.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	RecastWorldComponent::RecastWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_scene(nullptr) {}

	// ---------------------------------------------------

	void RecastWorldComponent::OnFixedRateTickLate(JobExecutor& executor, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/LateTick", "Update Recast navigation", 0xBBBBBB);
		_scene->Simulate(AsFloat(duration) / 1000000.0f /*Seconds per microsecond*/);
	}

}}} // namespace Eldritch2::Navigation::Recast
