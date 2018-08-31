/*==================================================================*\
  Window.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Win32Tools.hpp>
#include <Common/Algorithms.hpp>
#include <Common/ErrorCode.hpp>
#include <Common/Window.hpp>
#include <Common/Assert.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <resources/Eldritch2.Resources.h>
//------------------------------------------------------------------//
#include <eastl/utility.h>
#include <Windows.h>
#include <process.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	enum : DWORD {
		WM_CustomClose = WM_USER
	};

	static ETPureFunctionHint ATOM RegisterWindowClass(const wchar_t* className, WNDPROC windowProc) {
		const HINSTANCE thisInstance(GetModuleByAddress());
		WNDCLASSEXW     windowClass;

		windowClass.cbSize        = sizeof(WNDCLASSEXW);
		windowClass.style         = CS_OWNDC;
		windowClass.lpfnWndProc   = windowProc;
		windowClass.hInstance     = GetModuleByAddress(reinterpret_cast<void*>(windowProc));
		windowClass.cbClsExtra    = 0;
		windowClass.cbWndExtra    = 0;
		windowClass.hIcon         = static_cast<HICON>(LoadImageW(
            thisInstance,
            MAKEINTRESOURCEW(IDI_ICON1),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXICON),
            GetSystemMetrics(SM_CYICON),
            LR_DEFAULTCOLOR));
		windowClass.hCursor       = nullptr;
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName  = nullptr;
		windowClass.lpszClassName = className;
		windowClass.hIconSm       = static_cast<HICON>(LoadImageW(
            thisInstance,
            MAKEINTRESOURCEW(IDI_ICON1),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON),
            LR_DEFAULTCOLOR));

		const ATOM result(RegisterClassExW(&windowClass));

		ET_ASSERT(result != 0, "Error registering application window class!");

		return result;
	}

	// ---------------------------------------------------

	static ETPureFunctionHint unsigned int ETStdCall MessageThreadEntryPoint(void* windowPointer) {
		static const ATOM windowAtom(RegisterWindowClass(L"Eldritch2", [](HWND window, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT {
			switch (message) {
			case WM_SETCURSOR: {
				if (LOWORD(lParam) == HTCLIENT) {
					//	Hide the cursor.
					SetCursor(nullptr);

					return TRUE;
				}
			} // case WM_SETCURSOR

				/*	We deliberately ignore the system close event in favor of our custom one (an alias for WM_USER). This is done to avoid
			 *	the scenario where the user closes the window and the system frees the resources associated with said window while we
			 *	still need it for use by the renderer, etc.*/
			case WM_CLOSE: {
				return 0;
			} // case WM_CLOSE

			case WM_CustomClose: {
				PostQuitMessage(int(wParam));
			}  // case WM_CustomClose
			}; // switch( message )

			return DefWindowProcW(window, message, wParam, lParam);
		}));

		// ---

		const HWND window(CreateWindowExW(
			0L,
			reinterpret_cast<LPCWSTR>(windowAtom),
			WPROJECT_NAME,
			WS_OVERLAPPED,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			nullptr,
			nullptr,
			GetModuleByAddress(nullptr),
			nullptr));

		//	Publish readiness to the world.
		static_cast<Atomic<HWND>*>(windowPointer)->store(window, std::memory_order_release);
		if (!window) {
			return GetLastError();
		}

		MSG message;
		/*	Create the thread message queue. This is done so that the thread can be terminated cleanly with the help of PostThreadMessage() and WM_QUIT.
		 *	See http://msdn.microsoft.com/en-us/library/windows/desktop/ms644946(v=vs.85).aspx */
		PeekMessageW(&message, nullptr, WM_USER, WM_USER, PM_NOREMOVE);

		while (GetMessageW(&message, window, 0u, 0u) != 0) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		DestroyWindow(window);

		/*	This will contain the WPARAM value sent by PostQuitMessage() once we have exited the dispatch loop above.
		 *	See https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
		 *	See https://msdn.microsoft.com/en-us/library/windows/desktop/ms644945(v=vs.85).aspx */
		return message.wParam;
	}

} // anonymous namespace

Window::Window() :
	_window(nullptr),
	_messageThread(nullptr) {}

// ---------------------------------------------------

Window::Window(Window&& window) :
	_window(eastl::exchange(window._window, nullptr)),
	_messageThread(eastl::exchange(window._messageThread, nullptr)) {}

// ---------------------------------------------------

Window::~Window() {
	FreeResources();
}

// ---------------------------------------------------

DisplayMode Window::GetDisplayMode() const {
	RECT clientArea;

	if (GetClientRect(_window, &clientArea) == FALSE) {
		return { _mode, 0, 0 };
	}

	return { _mode, static_cast<uint16>(clientArea.right - clientArea.left), static_cast<uint16>(clientArea.bottom - clientArea.top) };
}

// ---------------------------------------------------

void Window::SetDisplayMode(DisplayMode mode) {
	enum : DWORD {
		WindowedBehaviors   = SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER,
		FullscreenBehaviors = SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW,

		WindowedStyle   = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,
		FullscreenStyle = WS_POPUP
	};

	// ---

	const HWND  parentWindow(mode.mode == FullscreenMode::Windowed ? nullptr : HWND_TOPMOST);
	const DWORD style(mode.mode == FullscreenMode::Windowed ? WindowedStyle : FullscreenStyle);
	const UINT  behaviors(mode.mode == FullscreenMode::Windowed ? WindowedBehaviors : FullscreenBehaviors);
	RECT        clientArea;

	clientArea.left   = 0;
	clientArea.top    = 0;
	clientArea.right  = mode.mode != FullscreenMode::Windowed ? GetSystemMetrics(SM_CXFULLSCREEN) : mode.widthInPixels;
	clientArea.bottom = mode.mode != FullscreenMode::Windowed ? GetSystemMetrics(SM_CYFULLSCREEN) : mode.heightInPixels;

	AdjustWindowRect(&clientArea, style, FALSE);

	SetWindowLongPtrW(_window, GWL_STYLE, style);
	SetWindowPos(_window, parentWindow, clientArea.left, clientArea.top, clientArea.right, clientArea.bottom, behaviors);
}

// ---------------------------------------------------

void Window::EnsureVisible() {
	SetWindowPos(_window, HWND_TOP, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
}

// ---------------------------------------------------

bool Window::Exists() const {
	return _window != nullptr;
}

// ---------------------------------------------------

ErrorCode Window::BindResources() {
	//	This value intentionally not legal.
	static const HWND WaitingForResponse(reinterpret_cast<HWND>(-1L));
	Atomic<HWND>      window(WaitingForResponse);

	const HANDLE thread(reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 4096u, &MessageThreadEntryPoint, &window, 0, nullptr)));
	if (thread == nullptr) {
		return Error::Unspecified;
	}

	while (window.load(std::memory_order_consume) == WaitingForResponse) {
		//	Busy wait while the new thread creates the window.
	}

	_window        = window.load(std::memory_order_relaxed);
	_messageThread = thread;

	return Error::None;
}

// ---------------------------------------------------

void Window::FreeResources() {
	if (const HWND window = eastl::exchange(_window, nullptr)) {
		/*	Send the custom close message to cause the window to shut down. This will in turn
		 *	cause the message pump thread to terminate. */
		SendMessageW(window, WM_CustomClose, 0, 0);
	}

	if (const HANDLE messageThread = eastl::exchange(_messageThread, nullptr)) {
		WaitForSingleObject(messageThread, INFINITE);
		CloseHandle(messageThread);
	}
}

// ---------------------------------------------------

HINSTANCE Window::GetHinstance() const {
	return reinterpret_cast<HINSTANCE>(GetWindowLongPtrW(_window, GWLP_HINSTANCE));
}

// ---------------------------------------------------

HWND Window::GetHwnd() const {
	return _window;
}

// ---------------------------------------------------

void Swap(Window& lhs, Window& rhs) {
	using ::Eldritch2::Swap;

	Swap(lhs._mode, rhs._mode);
	Swap(lhs._window, rhs._window);
	Swap(lhs._messageThread, rhs._messageThread);
}

} // namespace Eldritch2
