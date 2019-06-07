/*==================================================================*\
  Window.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Build.hpp>
//------------------------------------------------------------------//
#include <resources/Eldritch2.Resources.h>
//------------------------------------------------------------------//
#include <Windows.h>
#include <process.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	enum : DWORD {
		WM_AllocateWindow = WM_USER,
		WM_FreeWindow
	};

	static Atomic<HANDLE> UiThread(nullptr);
	static Atomic<DWORD>  UiThreadId(0u);
	static Atomic<bool>   IsActiveApplication(false);

	// ---------------------------------------------------

	static ETPureFunctionHint ATOM RegisterWindowClass(LPCWSTR name, LPCWSTR icon, WNDPROC windowProc) ETNoexceptHint {
		const HINSTANCE   instance(GetModuleByAddress());
		const WNDCLASSEXW windowClass{
			/*cbSize =*/sizeof(WNDCLASSEXW),
			/*style =*/CS_OWNDC,
			/*lpfnWndProc =*/windowProc,
			/*cbClsExtra =*/0,
			/*cbWndExtra =*/0,
			/*hInstance =*/instance,
			/*hIcon =*/HICON(LoadImageW(instance, icon, IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR)),
			/*hCursor =*/nullptr,
			/*hbrBackground =*/nullptr,
			/*lpszMenuName =*/nullptr,
			/*lpszClassName =*/name,
			/*hIconSm =*/HICON(LoadImageW(instance, icon, IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR)),
		};

		return RegisterClassExW(ETAddressOf(windowClass));
	}

} // anonymous namespace

Window::Window() ETNoexceptHint : _window(nullptr) {}

// ---------------------------------------------------

Window::Window(Window&& window) ETNoexceptHint : Window() {
	Swap(*this, window);
}

// ---------------------------------------------------

Window::~Window() {
	FreeResources();
}

// ---------------------------------------------------

DisplayMode Window::GetDisplayMode() const {
	RECT area;
	if (ET_UNLIKELY(GetClientRect(GetHwnd(), ETAddressOf(area)) == FALSE)) {
		return { _mode, 0, 0 };
	}

	return { _mode, uint16(area.right - area.left), uint16(area.bottom - area.top) };
}

// ---------------------------------------------------

void Window::SetDisplayMode(DisplayMode mode) {
	const DWORD style(mode.mode == FullscreenMode::Windowed ? (WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME) : WS_POPUP);
	const HWND  parent(mode.mode == FullscreenMode::Windowed ? nullptr : HWND_TOPMOST);
	const UINT  behaviors(mode.mode == FullscreenMode::Windowed ? (SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER) : (SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW));
	RECT        area{
        /*left =*/0,
        /*top =*/0,
        /*right =*/mode.mode != FullscreenMode::Windowed ? GetSystemMetrics(SM_CXFULLSCREEN) : mode.widthInPixels,
        /*bottom =*/mode.mode != FullscreenMode::Windowed ? GetSystemMetrics(SM_CYFULLSCREEN) : mode.heightInPixels
	};

	AdjustWindowRect(ETAddressOf(area), style, /*bMenu =*/FALSE);
	SetWindowLongPtrW(_window, GWL_STYLE, style);
	SetWindowPos(_window, parent, area.left, area.top, area.right, area.bottom, behaviors);
}

// ---------------------------------------------------

void Window::EnsureVisible() {
	SetWindowPos(_window, HWND_TOP, /*X =*/0, /*Y =*/0, /*cX =*/0, /*cY =*/0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
}

// ---------------------------------------------------

bool Window::Exists() const ETNoexceptHint {
	return _window != nullptr;
}

// ---------------------------------------------------

Result Window::BindResources() {
	ETAssert(UiThread.load(std::memory_order_acquire) != nullptr, "UI thread must be initialized before creating application windows!");

	//	This value intentionally nonsense/illegal.
	Atomic<HWND> window(HWND(-1L));

	ETVerify(PostThreadMessageW(UiThreadId, WM_AllocateWindow, WPARAM(ETAddressOf(window)), /*lParam=*/0) != FALSE, "Error sending create message to UI thread");
	while (window.load(std::memory_order_consume) == HWND(-1L)) {
		//	Busy wait while the UI thread creates the window.
	}

	if (const HWND old = Exchange(_window, window.load(std::memory_order_consume))) {
		PostThreadMessageW(UiThreadId, WM_FreeWindow, WPARAM(old), /*lParam=*/0);
	}

	return Result::Success;
}

// ---------------------------------------------------

void Window::FreeResources() {
	if (const HWND window = Exchange(_window, nullptr)) {
		PostThreadMessageW(UiThreadId, WM_FreeWindow, WPARAM(window), /*lParam=*/0);
	}
}

// ---------------------------------------------------

HINSTANCE Window::GetHinstance() const ETNoexceptHint {
	return HINSTANCE(GetWindowLongPtrW(_window, GWLP_HINSTANCE));
}

// ---------------------------------------------------

HWND Window::GetHwnd() const ETNoexceptHint {
	return _window;
}

// ---------------------------------------------------

void Swap(Window& lhs, Window& rhs) ETNoexceptHint {
	using ::Eldritch2::Swap;

	Swap(lhs._mode, rhs._mode);
	Swap(lhs._window, rhs._window);
}

// ---------------------------------------------------

bool IsForegroundApplication(MemoryOrder order) {
	return IsActiveApplication.load(order);
}

// ---------------------------------------------------

Result InitializeUiThread() {
	const auto MessageThreadEntryPoint([](void* /*unused*/) ETNoexceptHint -> unsigned int {
		const ATOM windowAtom(RegisterWindowClass(SL("Eldritch2.UiWindow"), MAKEINTRESOURCEW(IDI_ICON1), [](HWND window, UINT message, WPARAM wParam, LPARAM lParam) ETNoexceptHint -> LRESULT {
			switch (message) {
			case WM_ACTIVATEAPP: {
				IsActiveApplication.store(wParam == TRUE, std::memory_order_release);
				return 0;
			} // case WM_ACTIVATEAPP
			case WM_SETCURSOR: {
				if (LOWORD(lParam) == HTCLIENT) {
					//	Hide the cursor.
					SetCursor(nullptr);
					return TRUE;
				}
			} // case WM_SETCURSOR

			/*	We deliberately ignore the system close event. This is done to avoid the scenario where the user closes the window and the system frees
			 *	the resources associated with this window while the application still needs them for use by the renderer, etc.*/
			case WM_CLOSE: {
				return 0;
			}  // case WM_CLOSE
			}; // switch( message )

			return DefWindowProcW(window, message, wParam, lParam);
		}));

		MSG message;
		/*	Create the thread message queue. This is done so that the thread can be terminated cleanly with the help of PostThreadMessage() and WM_QUIT.
		*	See http://msdn.microsoft.com/en-us/library/windows/desktop/ms644946(v=vs.85).aspx */
		PeekMessageW(ETAddressOf(message), /*hWnd =*/nullptr, WM_USER, WM_USER, PM_NOREMOVE);

		while (GetMessageW(ETAddressOf(message), /*hWnd =*/nullptr, /*wMsgFilterMin =*/0u, /*wMsgFilterMax =*/0u) != 0) {
			if (message.hwnd == nullptr && message.message == WM_AllocateWindow) {
				reinterpret_cast<Atomic<HWND>*>(message.wParam)->store(CreateWindowExW(
																		   /*dwExStyle =*/0L, LPCWSTR(windowAtom), PROJECT_NAME_LIT, WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
																		   /*hWndParent =*/nullptr,
																		   /*hMenu =*/nullptr, GetModuleByAddress(nullptr), nullptr),
																	   std::memory_order_release);
				continue;
			}

			if (message.hwnd == nullptr && message.message == WM_FreeWindow) {
				DestroyWindow(HWND(message.wParam));
				continue;
			}

			TranslateMessage(ETAddressOf(message));
			DispatchMessageW(ETAddressOf(message));
		}

		/*	This will contain the WPARAM value sent by PostQuitMessage() once we have exited the dispatch loop above.
		 *	See https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
		 *	See https://msdn.microsoft.com/en-us/library/windows/desktop/ms644945(v=vs.85).aspx */
		return message.wParam;
	});

	ETAssert(UiThread.load(std::memory_order_acquire) == nullptr, "UI thread must be initialized exactly once!");
	const HANDLE thread(HANDLE(_beginthreadex(/*_Security =*/nullptr, /*StackSize =*/4096u, MessageThreadEntryPoint, /*_ArgList =*/nullptr, 0, nullptr)));

	return thread != nullptr ? Result::Success : Result::Unspecified;
}

// ---------------------------------------------------

void TerminateUiThread() {
	const HANDLE thread(UiThread.exchange(nullptr, std::memory_order_consume));
	if (thread == nullptr) {
		return;
	}

	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
}

} // namespace Eldritch2
