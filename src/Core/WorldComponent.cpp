/*==================================================================*\
  WorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;

	void WorldComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/BindResourcesLate", "Default early initialization function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::BindResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/BindResourcesEarly", "Default late initialization function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::FreeResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/FreeResources", "Default tear down function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnVariableRateTick(JobExecutor& executor, MicrosecondTime tickDuration, float32 residualFraction) {
		ET_PROFILE_SCOPE("World/VariableTick", "Default variable-rate tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnFixedRateTickEarly(JobExecutor& executor, MicrosecondTime delta) {
		ET_PROFILE_SCOPE("World/EarlyTick", "Default early tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnFixedRateTick(JobExecutor& executor, MicrosecondTime delta) {
		ET_PROFILE_SCOPE("World/Tick", "Default standard tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::OnFixedRateTickLate(JobExecutor& executor, MicrosecondTime delta) {
		ET_PROFILE_SCOPE("World/LateTick", "Default late tick function", 0xA011F0);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::DefineScriptApi(ApiBuilder& /*api*/) {
		ET_PROFILE_SCOPE("World/Initialization", "Default Wren script API registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void WorldComponent::PublishServices(ObjectLocator& /*services*/) {
		ET_PROFILE_SCOPE("World/Initialization", "Default service attach function", 0xA011F0);
		//	Default implementation should not do anything.
	}

}} // namespace Eldritch2::Core
