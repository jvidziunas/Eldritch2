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
#include <microprofile/microprofile.h>
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

	XInputEngineComponent::XInputEngineComponent(const Blackboard& services) :
		EngineComponent(services) {}

	// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> XInputEngineComponent::CreateWorldComponent(Allocator& allocator, const World& world) {
		UniquePointer<WorldComponent> inputComponent(MakeUnique<XInputWorldComponent>(allocator, world));

		if (inputComponent == nullptr) {
			return Error::OutOfMemory;
		}

		return eastl::move(inputComponent);
	}

	// ---------------------------------------------------

	void XInputEngineComponent::AcceptVisitor(JobExecutor& /*executor*/, const ServiceTickVisitor) {
		MICROPROFILE_SCOPEI("Engine/ServiceTick", "Sample XInput pad state", 0xFFFFFF);

		for (DWORD pad(0u); pad < _countof(_gamepads); ++pad) {
			XInputGetState(pad, &_gamepads[pad]);
		}
	}

}}} // namespace Eldritch2::Input::XInput
