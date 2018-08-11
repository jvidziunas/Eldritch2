/*==================================================================*\
  PhysxWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXWorldComponent.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {
	using namespace ::physx;

	namespace {

		static ETInlineHint ETPureFunctionHint PxReal AsSeconds(MicrosecondTime microseconds) {
			return PxReal(AsFloat(microseconds) / /*seconds per microsecond*/ 1000000.0f);
		}

	} // anonymous namespace

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	PhysxWorldComponent::PhysxWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		CpuDispatcher(4u),
		_log(FindService<World>().GetLog()),
		_shouldJoinScene(false),
		_scene(nullptr) {
	}

	// ---------------------------------------------------

	void PhysxWorldComponent::OnFixedRateTickEarly(JobExecutor& executor, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/EarlyTick/PhysX", "Join simulation", 0x32AACD);
		const bool shouldJoin(eastl::exchange(_shouldJoinScene, false));

		if (shouldJoin && Failed(_scene->JoinSimulation(executor))) {
			World& world(FindService<World>());

			_log.Write(MessageType::Error, "PhysX hardware error simulating world {}, terminating." UTF8_NEWLINE, fmt::ptr(&world));
			world.SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void PhysxWorldComponent::OnFixedRateTickLate(JobExecutor& executor, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/LateTick/PhysX", "Start simulation", 0x32AACD);
		_scene->BeginSimulation(executor, AsSeconds(duration));
		_shouldJoinScene = true;
	}

}}} // namespace Eldritch2::Physics::PhysX
