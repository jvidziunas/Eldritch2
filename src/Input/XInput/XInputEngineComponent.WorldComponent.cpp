/*==================================================================*\
  XInputEngineComponent.WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/XInputEngineComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace XInput {

	using namespace ::Eldritch2::Scheduling;

	// ---------------------------------------------------

	XInputEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services), _gamepads(nullptr) {}

	// ---------------------------------------------------

	void XInputEngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) {
		//	ET_TERMINATE_WORLD_UNLESS(_gamepads && Succeeded(_gamepads->BindResources()));
	}

}}} // namespace Eldritch2::Input::XInput
