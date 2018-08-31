/*==================================================================*\
  PhysxEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Physics/PhysX/PhysxWorldComponent.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <foundation/PxFoundationVersion.h>
#include <common/PxTolerancesScale.h>
#include <foundation/PxFoundation.h>
#include <PxPhysicsVersion.h>
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_DEBUGGABLE_LIBRARY("PxFoundationDEBUG_x64.lib", "PxFoundation_x64.lib");
ET_LINK_DEBUGGABLE_LIBRARY("PhysX3ExtensionsDEBUG.lib", "PhysX3Extensions.lib");
ET_LINK_DEBUGGABLE_LIBRARY("PhysX3CommonDEBUG_x64.lib", "PhysX3Common_x64.lib");
ET_LINK_DEBUGGABLE_LIBRARY("PhysX3DEBUG_x64.lib", "PhysX3_x64.lib");
ET_LINK_DEBUGGABLE_LIBRARY("PhysX3CharacterKinematicDEBUG_x64.lib", "PhysX3CharacterKinematic_x64.lib");
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	PhysxEngineComponent::PhysxEngineComponent(const ObjectLocator& services) :
		EngineComponent(services),
		_log(FindService<Engine>()->GetLog()),
		_allocator("PhysX Root Allocator") {}

	// ---------------------------------------------------

	UniquePointer<WorldComponent> PhysxEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<PhysxWorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void PhysxEngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Initialization/PhysX", "PhysX SDK creation", 0x76b900);

		if (!PxCreateFoundation(PX_FOUNDATION_VERSION, _allocator, _log)) {
			_log.Write(Severity::Error, "Error creating PhysX foundation object!" ET_NEWLINE);
			FindService<Engine>()->SetShouldShutDown();
			return;
		}

		if (!PxCreatePhysics(PX_PHYSICS_VERSION, PxGetFoundation(), PxTolerancesScale())) {
			_log.Write(Severity::Error, "Error creating PhysX object!" ET_NEWLINE);
			FindService<Engine>()->SetShouldShutDown();
			return;
		}

		_log.Write(Severity::Message, "PhysX SDK initialized successfully." ET_NEWLINE);
	}

}}} // namespace Eldritch2::Physics::PhysX
