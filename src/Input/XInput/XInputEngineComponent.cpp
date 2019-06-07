/*==================================================================*\
  XInputEngineComponent.cpp
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
#include <Input/XInput/XInputEngineComponent.hpp>
#include <Core/Profiler.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if defined(XINPUT_USE_9_1_0)
ET_LINK_LIBRARY("XInput9_1_0.lib")
#else
ET_LINK_LIBRARY("XInput.lib")
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace XInput {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	XInputEngineComponent::XInputEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services) {}

	// ---------------------------------------------------

	void XInputEngineComponent::TickEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Sample XInput pad state", 0xFFFFFF);

		for (DWORD pad(0u); pad < ETCountOf(_gamepads); ++pad) {
			XInputGetState(pad, ETAddressOf(_gamepads[pad]));
		}
	}

}}} // namespace Eldritch2::Input::XInput
