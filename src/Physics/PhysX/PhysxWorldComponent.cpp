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

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	PhysxWorldComponent::PhysxWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		CpuDispatcher(4u),
		_log(FindService<World>()->GetLog()),
		_scene(nullptr) {
	}

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

	void PhysxWorldComponent::OnFixedRateTickLate(JobExecutor& executor, MicrosecondTime duration) {
		ET_PROFILE_SCOPE("World/LateTick/PhysX", "Start simulation", 0x32AACD);
		if (Failed(_scene->StartSimulation(AsSeconds(duration)))) {
			FindService<World>()->SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void PhysxWorldComponent::BindResources(JobExecutor& /*executor*/) {}

	// ---------------------------------------------------

	void PhysxWorldComponent::FreeResources(JobExecutor& executor) {
		if (_scene) {
			executor.AwaitCondition(_scene->IsSimulationComplete());
		}

		executor.AwaitFence(GetTasksCompletedFence());
	}

}}} // namespace Eldritch2::Physics::PhysX
