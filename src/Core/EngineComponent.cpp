/*==================================================================*\
  EngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
#include <Core/Profiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;

	void EngineComponent::TickEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Tick", "Default early tick", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::Tick(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Tick", "Default standard tick", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default early-phase initialization", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default configuration broadcast", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/LateInitialization", "Default late-phase initialization", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::FreeResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Cleanup", "Default cleanup", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::PublishApi(PropertyApiBuilder& /*api*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default property API registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::PublishApi(AssetApiBuilder& /*api*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default asset API registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::PublishApi(ObjectInjector& /*api*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default dependency injection API registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

}} // namespace Eldritch2::Core
