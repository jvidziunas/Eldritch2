/*==================================================================*\
  Win32InputEngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Input/Win32/Win32InputWorldComponent.hpp>
#include <Scheduling/JobSystem.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <Windowsx.h>
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	namespace {

		static LRESULT CALLBACK WindowsKeyHook(int nCode, WPARAM wParam, LPARAM lParam) {
			if (ET_LIKELY(nCode >= HC_ACTION)) {
				switch (wParam) {
				case WM_KEYDOWN:
				case WM_KEYUP: {
					const KBDLLHOOKSTRUCT& hook(*reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam));
					const bool             isWindowsKey((VK_LWIN == hook.vkCode) | (VK_RWIN == hook.vkCode));
					const bool             isRealKeypress(!(hook.dwExtraInfo & LLMHF_INJECTED));
					/*	Verify that this was an actual keypress, *not* a specially-injected Windows key event.
						 *	We're interested in stopping the cases whereupon a user accidentally fat-fingers the Windows key
						 *	when they actually meant to press the control or alt keys. */
					if (ET_UNLIKELY(isRealKeypress & isWindowsKey)) {
						return 1;
					}
				} // case WM_KEYDOWN, case WM_KEYUP
				} // switch( wParam )
			}

			return CallNextHookEx(nullptr, nCode, wParam, lParam);
		}

	} // anonymous namespace

	// ---------------------------------------------------

	Win32InputEngineComponent::Win32InputEngineComponent(
		const Blackboard& services,
		Log&              log) :
		EngineComponent(services),
		_keyboardHook(SetWindowsHookExW(WH_KEYBOARD_LL, &WindowsKeyHook, GetModuleByAddress(&WindowsKeyHook), 0)),
		_devices(log),
		_inputReader(_devices) {
	}

	// ---------------------------------------------------

	Win32InputEngineComponent::~Win32InputEngineComponent() {
		_inputReader.AwaitCompletion();

		//	Remove the hook that allowed us to selectively ignore Windows key press events.
		if (_keyboardHook) {
			UnhookWindowsHookEx(_keyboardHook);
		}
	}

	// ---------------------------------------------------

	Result<UniquePointer<WorldComponent>> Win32InputEngineComponent::CreateWorldComponent(Allocator& allocator, const World& world) {
		UniquePointer<WorldComponent> inputComponent(MakeUnique<Win32InputWorldComponent>(allocator, world));
		if (inputComponent == nullptr) {
			return Error::OutOfMemory;
		}

		return eastl::move(inputComponent);
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::AcceptVisitor(JobExecutor& /*executor*/, const InitializationVisitor) {
		MICROPROFILE_SCOPEI("Engine/Initialization", "Initialize Win32 input", 0xBBBBBB);

		if (Failed(FindService<JobSystem>().Launch(_inputReader))) {
			return FindService<Engine>().SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::AcceptVisitor(Blackboard& services) {
		services.Publish<DeviceCoordinator>(_devices);
	}

}}} // namespace Eldritch2::Input::Win32
