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
#include <Scheduling/JobExecutor.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

#define ET_TERMINATE_WORLD_UNLESS(cond)                  \
	if (!(cond)) {                                       \
		FindService<Core::World>()->SetShouldShutDown(); \
	}

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	PhysxWorldComponent::PhysxWorldComponent(const ObjectLocator& services) ETNoexceptHint : WorldComponent(services), CpuDispatcher(4u), _scene(nullptr) {}

	// ---------------------------------------------------

	void PhysxWorldComponent::OnFixedRateTickEarly(JobExecutor& executor, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/EarlyTick/PhysX", "Join simulation", 0x32AACD);
		executor.AwaitCondition(_scene->IsSimulationComplete());

		if (Failed(_scene->FinishSimulation())) {
			_log.Write(Severity::Error, "PhysX hardware error simulating scene {}, terminating." ET_NEWLINE, fmt::ptr(_scene));
			FindService<World>()->SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void PhysxWorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/LateTick/PhysX", "Start simulation", 0x32AACD);

		_scene->Simulate(duration);
	}

	// ---------------------------------------------------

	void PhysxWorldComponent::BindResources(JobExecutor& /*executor*/) {
		_log.BindResources(FindService<World>()->GetLog());

		ET_TERMINATE_WORLD_UNLESS(_scene && Succeeded(_scene->BindResources(*this)));
	}

	// ---------------------------------------------------

	void PhysxWorldComponent::FreeResources(JobExecutor& executor) {
		if (PhysicsScene* const scene = eastl::exchange(_scene, nullptr)) {
			executor.AwaitCondition(scene->IsSimulationComplete());
			scene->FreeResources();
		}

		executor.AwaitFence(GetTasksCompletedFence());
	}

}}} // namespace Eldritch2::Physics::PhysX
