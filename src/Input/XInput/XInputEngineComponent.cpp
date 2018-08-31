/*==================================================================*\
  XInputEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/XInputEngineComponent.hpp>
#include <Input/XInput/XInputWorldComponent.hpp>
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

	XInputEngineComponent::XInputEngineComponent(const ObjectLocator& services) :
		EngineComponent(services) {}

	// ---------------------------------------------------

	UniquePointer<WorldComponent> XInputEngineComponent::CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) {
		return MakeUnique<XInputWorldComponent>(allocator, services);
	}

	// ---------------------------------------------------

	void XInputEngineComponent::TickEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/ServiceTick", "Sample XInput pad state", 0xFFFFFF);
		for (DWORD pad(0u); pad < ETCountOf(_gamepads); ++pad) {
			XInputGetState(pad, &_gamepads[pad]);
		}
	}

}}} // namespace Eldritch2::Input::XInput
