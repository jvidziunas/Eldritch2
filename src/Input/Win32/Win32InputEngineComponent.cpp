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
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Core/Profiler.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <Windowsx.h>
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint LRESULT CALLBACK WindowsKeyHook(int nCode, WPARAM wParam, LPARAM lParam) ETNoexceptHint {
			if (ET_LIKELY(nCode >= HC_ACTION)) {
				switch (wParam) {
				case WM_KEYDOWN:
				case WM_KEYUP: {
					const auto hook{ LPKBDLLHOOKSTRUCT(lParam) };
					const bool isWindowsKey((VK_LWIN == hook->vkCode) | (VK_RWIN == hook->vkCode));
					const bool isRealKeypress(!(hook->dwExtraInfo & LLMHF_INJECTED));
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

	Win32InputEngineComponent::Win32InputEngineComponent(const ObjectInjector& services) ETNoexceptHint : EngineComponent(services),
																										 _keyboardHook(SetWindowsHookExW(WH_KEYBOARD_LL, &WindowsKeyHook, GetModuleByAddress(&WindowsKeyHook), 0)),
																										 _inputReader(_devices) {}

	// ---------------------------------------------------

	Win32InputEngineComponent::~Win32InputEngineComponent() {
		_inputReader.AwaitCompletion();

		if (_keyboardHook) {
			//	Remove the hook that allowed us to selectively ignore Windows key press events.
			UnhookWindowsHookEx(_keyboardHook);
		}
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Initialize Win32 input", 0xBBBBBB);

		// _devices.BindResources(*FindService<Log>());
		ET_TERMINATE_WORLD_IF_FAILED(_inputReader.Boot("Win32 Input Reader Thread"));
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::PublishApi(ObjectInjector& api) {
		api.PublishService<DeviceCoordinator>(_devices);
	}

}}} // namespace Eldritch2::Input::Win32
