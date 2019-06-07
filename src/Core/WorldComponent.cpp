/*==================================================================*\
  WorldComponent.hpp
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
#include <Core/WorldComponent.hpp>
#include <Core/Profiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;

	// ---------------------------------------------------

	void WorldComponent::BindResourcesEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/BindResourcesLate", "Default early initialization function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/BindResourcesEarly", "Default late initialization function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::FreeResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/FreeResources", "Default tear down function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnVariableRateTick(JobExecutor& /*executor*/, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/VariableTick", "Default variable-rate tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnFixedRateTickEarly(JobExecutor& /*executor*/, MicrosecondTime /*delta*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/EarlyTick", "Default early tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnFixedRateTick(JobExecutor& /*executor*/, MicrosecondTime /*delta*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/Tick", "Default standard tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnFixedRateTickLate(JobExecutor& /*executor*/, MicrosecondTime /*delta*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/LateTick", "Default late tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::PublishApi(ApiBuilder&) {
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::PublishApi(ObjectInjector&) {
		//	Default implementation should not do anything.
	}

}} // namespace Eldritch2::Core
