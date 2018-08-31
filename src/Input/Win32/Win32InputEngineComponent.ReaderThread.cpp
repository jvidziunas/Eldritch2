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

	namespace {

		ETInlineHint LPCWSTR RegisterWindowClass(LPCWSTR className, WNDPROC windowProc, HINSTANCE instance) {
			WNDCLASSEXW windowClass {
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

			return LPCWSTR(RegisterClassExW(&windowClass));
		}

		// ---------------------------------------------------

		static ETInlineHint bool RegisterListeners(HWND window) {
			enum : DWORD { AttachFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY };
			enum : USHORT {
				KeyboardPage  = 0x01,
				KeyboardUsage = 0x06,
				MousePage     = 0x01,
				MouseUsage    = 0x02
			};

			const RAWINPUTDEVICE listeners[] = {
				{ KeyboardPage, KeyboardUsage, AttachFlags, window },
				{ MousePage, MouseUsage, AttachFlags, window }
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

	ErrorCode Win32InputEngineComponent::ReaderThread::EnterOnCaller() {
		/*	Since we operate in short bursts of work before sleeping, boost thread priority slightly to reduce the amount of downtime before the
		 *	scheduler will queue us for execution. Windows internally does some priority boosting for us behind the scenes when we receive input,
		 *	this is designed to supplement that behavior. */
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
		const LPCWSTR windowClass(RegisterWindowClass(L"MessageWindow", &DefWindowProcW, GetModuleByAddress(&DefWindowProcW)));
		ET_AT_SCOPE_EXIT(UnregisterClassW(windowClass, GetModuleByAddress(&DefWindowProcW)));

		const HWND window(CreateWindowExW(0, windowClass, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, nullptr, nullptr));
		ET_AT_SCOPE_EXIT(DestroyWindow(window));
		if (!RegisterListeners(window) || !_devices->Enumerate()) {
			_window.store(nullptr, std::memory_order_release);
			return Error::Unspecified;
		}

		//	Signal to the outside world that we have initialized.
		_window.store(window, std::memory_order_release);

		RAWINPUT input;
		BOOL     hadError;
		//	GetMessage() returns 0 if it recieved a WM_QUIT message. This call intentionally blocks.
		for (MSG event; ET_LIKELY(hadError = GetMessageW(&event, window, 0u, 0u)) != 0; /*no increment*/) {
			if (ET_UNLIKELY(hadError == -1)) {
				break;
			}

			if (ET_LIKELY(event.message == WM_INPUT)) {
				PRAWINPUT ptr(&input);
				UINT      size(sizeof(input));

				if (ET_LIKELY(GetRawInputData(HRAWINPUT(event.lParam), RID_INPUT, ptr, &size, sizeof(RAWINPUTHEADER)) > 0)) {
					_devices->Route(input);
				}

				//	This seems to be a no-op in all versions of Windows, but we do it just to be safe.
				DefRawInputProc(&ptr, 1, sizeof(RAWINPUTHEADER));
			}

			TranslateMessage(&event);
			DispatchMessageW(&event);
		}

		return Error::None;
	}

	// ---------------------------------------------------

	void Win32InputEngineComponent::ReaderThread::SetShouldShutDown() ETNoexceptHint {
		if (const HWND window = _window.exchange(nullptr, std::memory_order_acquire)) {
			PostMessageW(window, WM_CLOSE, 0, 0);
		}
	}

}}} // namespace Eldritch2::Input::Win32
