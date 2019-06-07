/*==================================================================*\
  Win32InputEngineComponent.ReaderThread.cpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

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
#include <Input/Win32/Win32InputEngineComponent.hpp>
//------------------------------------------------------------------//
#include <windowsx.h>
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint LPCWSTR RegisterWindowClass(LPCWSTR className, WNDPROC windowProc, HINSTANCE instance) ETNoexceptHint {
			WNDCLASSEXW windowClass{
				/*cbSize =*/sizeof(WNDCLASSEXW),
				/*style =*/0,
				/*lpfnWndProc =*/windowProc,
				/*cbClsExtra =*/0u,
				/*cbWndExtra =*/0u,
				/*hInstance =*/instance,
				/*hIcon =*/nullptr,
				/*hCursor =*/nullptr,
				/*hbrBackground =*/nullptr,
				/*lpszMenuName =*/nullptr,
				/*lpszClassName =*/className,
				/*hIconSm =*/nullptr
			};

			return LPCWSTR(RegisterClassExW(ETAddressOf(windowClass)));
		}

		// ---------------------------------------------------

		ETForceInlineHint bool RegisterListeners(HWND window) ETNoexceptHint {
			enum : USHORT {
				KeyboardUsage = 0x06,
				MouseUsage    = 0x02
			};

			const RAWINPUTDEVICE listeners[] = {
				{ /*usUsagePage =*/0x01, KeyboardUsage, /*dwFlags =*/ RIDEV_INPUTSINK | RIDEV_NOLEGACY, window },
				{ /*usUsagePage =*/0x01, MouseUsage, /*dwFlags =*/RIDEV_INPUTSINK | RIDEV_NOLEGACY, window }
			};

			// ---

			if (window == nullptr) {
				return false;
			}

			return RegisterRawInputDevices(listeners, ETCountOf(listeners), sizeof(*listeners)) == TRUE;
		}

	} // anonymous namespace

	Win32InputEngineComponent::ReaderThread::ReaderThread(DeviceCoordinator& devices) ETNoexceptHint : _window(nullptr),
																									   _devices(ETAddressOf(devices)) {}

	// ---------------------------------------------------

	Result Win32InputEngineComponent::ReaderThread::EnterOnCaller() ETNoexceptHint {
		/*	Since we operate in short bursts of work before sleeping, boost thread priority slightly to reduce the amount of downtime before the
		 *	scheduler will queue us for execution. Windows internally does some priority boosting for us behind the scenes when we receive input,
		 *	this is designed to supplement that behavior. */
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		const LPCWSTR windowClass(RegisterWindowClass(L"MessageWindow", ETAddressOf(DefWindowProcW), GetModuleByAddress(ETAddressOf(DefWindowProcW))));
		ET_AT_SCOPE_EXIT(UnregisterClassW(windowClass, GetModuleByAddress(ETAddressOf(DefWindowProcW))));

		const HWND window(CreateWindowExW(0, windowClass, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr));
		ET_AT_SCOPE_EXIT(DestroyWindow(window));
		if (!RegisterListeners(window) || !_devices->Enumerate()) {
			_window.store(nullptr, std::memory_order_release);
			return Result::Unspecified;
		}

		//	Signal to the outside world that we have initialized.
		_window.store(window, std::memory_order_release);

		RAWINPUT input;
		BOOL     hadError;
		//	GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		for (MSG event; ET_LIKELY(hadError = GetMessageW(ETAddressOf(event), window, 0u, 0u)) != 0; /*no increment*/) {
			if (ET_UNLIKELY(hadError == -1)) {
				break;
			}

			if (ET_LIKELY(event.message == WM_INPUT)) {
				PRAWINPUT ptr(ETAddressOf(input));
				UINT      size(sizeof(input));

				if (ET_LIKELY(GetRawInputData(HRAWINPUT(event.lParam), RID_INPUT, ptr, ETAddressOf(size), sizeof(RAWINPUTHEADER)) > 0)) {
					_devices->Route(input);
				}

				//	This seems to be a no-op in all versions of Windows, but we do it just to be safe.
				DefRawInputProc(ETAddressOf(ptr), 1, sizeof(RAWINPUTHEADER));
			}

			TranslateMessage(ETAddressOf(event));
			DispatchMessageW(ETAddressOf(event));
		}

		return Result::Success;
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::ReaderThread::SetShouldShutDown() ETNoexceptHint {
		if (const HWND window = _window.exchange(nullptr, std::memory_order_acquire)) {
			PostMessageW(window, WM_CLOSE, 0, 0);
		}
	}

}}} // namespace Eldritch2::Input::Win32
