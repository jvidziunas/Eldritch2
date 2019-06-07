/*==================================================================*\
  PhysxEngineComponent.WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXEngineComponent.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Core/Profiler.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	// ---------------------------------------------------

	PhysxEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _scene(nullptr) {}

	// ---------------------------------------------------

	void PhysxEngineComponent::WorldComponent::OnFixedRateTickEarly(JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/EarlyTick/PhysX", "Join simulation", 0x32AACD);

		executor.AwaitCondition(_scene->IsSimulationComplete());
		ET_TERMINATE_WORLD_IF_FAILED(_scene->FinishSimulation());
	}

	// ---------------------------------------------------

	void PhysxEngineComponent::WorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime duration) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/LateTick/PhysX", "Start simulation", 0x32AACD);

		_scene->Simulate(duration);
	}

	// ---------------------------------------------------

	void PhysxEngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_TERMINATE_WORLD_UNLESS(_scene && Succeeded(_scene->BindResources(*this)));
	}

	// ---------------------------------------------------

	void PhysxEngineComponent::WorldComponent::FreeResources(JobExecutor& executor) ETNoexceptHint {
		executor.AwaitFence(GetTasksCompletedFence());
		
		if (PhysicsScene* const scene = Exchange(_scene, nullptr)) {
			executor.AwaitCondition(scene->IsSimulationComplete());
			scene->FreeResources();
		}
	}

}}} // namespace Eldritch2::Physics::PhysX
