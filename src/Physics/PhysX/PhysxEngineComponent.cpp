/*==================================================================*\
  PhysxEngineComponent.cpp
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
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Core/Profiler.hpp>
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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	// ---------------------------------------------------

	PhysxEngineComponent::PhysxEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services), _allocator("PhysX Root Allocator") {}

	// ---------------------------------------------------

	void PhysxEngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization/PhysX", "PhysX SDK creation", 0x76b900);

		ET_TERMINATE_ENGINE_IF_FAILED(_log.BindResources(*Inject<Log>()));
		ET_TERMINATE_ENGINE_UNLESS(PxCreateFoundation(PX_FOUNDATION_VERSION, _allocator, _log));
		ET_TERMINATE_ENGINE_UNLESS(PxCreatePhysics(PX_PHYSICS_VERSION, PxGetFoundation(), PxTolerancesScale()));

		_log.Write(Severity::Message, "PhysX SDK initialized successfully." ET_NEWLINE);
	}

}}} // namespace Eldritch2::Physics::PhysX
