/*==================================================================*\
  PlatformWindow.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Win32ApplicationHelpers.hpp>
#include <Scheduling/ThreadScheduler.hpp>
#include <Platform/PlatformWindow.hpp>
#include <Utility/ErrorCode.hpp>
#include <Utility/Assert.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <resources/Eldritch2.Resources.h>
//------------------------------------------------------------------//
#include <Windows.h>
#include <process.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;

namespace Eldritch2 {
namespace Platform {
namespace {

	enum : DWORD {
		WM_CustomClose	= WM_USER
	};

	static ETPureFunctionHint ATOM RegisterWindowClass( const wchar_t* className, WNDPROC windowProc ) {
		const HINSTANCE	thisInstance( Utility::GetInstance() );
		WNDCLASSEXW		windowClass;

		windowClass.cbSize			= sizeof(WNDCLASSEXW);
		windowClass.style			= CS_OWNDC;
		windowClass.lpfnWndProc		= windowProc;
		windowClass.hInstance		= Utility::GetInstance( reinterpret_cast<void*>(windowProc) );
		windowClass.cbClsExtra		= 0;
		windowClass.cbWndExtra		= 0;
		windowClass.hIcon			= static_cast<HICON>(LoadImageW( thisInstance,
																	 MAKEINTRESOURCEW( IDI_ICON1 ),
																	 IMAGE_ICON,
																	 GetSystemMetrics( SM_CXICON ),
																	 GetSystemMetrics( SM_CYICON ),
																	 LR_DEFAULTCOLOR ));
		windowClass.hCursor			= nullptr;
		windowClass.hbrBackground	= nullptr;
		windowClass.lpszMenuName	= nullptr;
		windowClass.lpszClassName	= className;
		windowClass.hIconSm			= static_cast<HICON>(LoadImageW( thisInstance,
																	 MAKEINTRESOURCEW( IDI_ICON1 ),
																	 IMAGE_ICON,
																	 GetSystemMetrics( SM_CXSMICON ),
																	 GetSystemMetrics( SM_CYSMICON ),
																	 LR_DEFAULTCOLOR ));

		const ATOM	result( RegisterClassExW( &windowClass ) );

		ETRuntimeAssert( 0 != result );

		return result;
	}

// ---------------------------------------------------

	static ETPureFunctionHint unsigned int ETStdCall MessageThreadEntryPoint( void* windowPointer ) {
		static const ATOM	windowAtom( RegisterWindowClass( L"Eldritch2", [] ( HWND window, UINT message, WPARAM wParam, LPARAM lParam ) -> LRESULT {
			switch( message ) {
				case WM_SETCURSOR: {
					if( LOWORD( lParam ) == HTCLIENT ) {
					//	Hide the cursor.
						SetCursor( nullptr );

						return TRUE;
					}
				}	// case WM_SETCURSOR

			/*	We deliberately ignore the system close event in favor of our custom one (an alias for WM_USER). This is done to avoid
			 *	the scenario where the user closes the window and the system frees the resources associated with said window while we
			 *	still need it for use by the renderer, etc.*/
				case WM_CLOSE: {
					return 0;
				}	// case WM_CLOSE

				case WM_CustomClose: {
					PostQuitMessage( wParam );
				}	// case WM_CustomClose
			};	// switch( message )

			return DefWindowProcW( window, message, wParam, lParam );
		} ) );

	// ---

		const auto	windowHandle( CreateWindowExW( 0L,
								  reinterpret_cast<LPCWSTR>( windowAtom ),
								  WPROJECT_NAME,
								  WS_OVERLAPPED,
								  CW_USEDEFAULT,
								  CW_USEDEFAULT,
								  0,
								  0,
								  nullptr,
								  nullptr,
								  Utility::GetInstance(),
								  nullptr ) );
		MSG			message;

	/*	Create the thread message queue. This is done so that the thread can be terminated cleanly with the help of PostThreadMessage() and WM_QUIT.
	 *	See http://msdn.microsoft.com/en-us/library/windows/desktop/ms644946(v=vs.85).aspx */
		PeekMessageW( &message, nullptr, WM_USER, WM_USER, PM_NOREMOVE );

		ETRuntimeAssert( nullptr != windowHandle );

	//	Publish readiness to the world.
		*static_cast<HWND*>( windowPointer ) = windowHandle;

		while( 0 != GetMessageW( &message, windowHandle, 0u, 0u ) ) {
			TranslateMessage( &message );
			DispatchMessageW( &message );
		}

		DestroyWindow( windowHandle );

	/*	This will contain the WPARAM value sent by PostQuitMessage() once we have exited the dispatch loop above.
	 *	See https://msdn.microsoft.com/en-us/library/windows/desktop/ms644936(v=vs.85).aspx
	 *	See https://msdn.microsoft.com/en-us/library/windows/desktop/ms644945(v=vs.85).aspx */
		return message.wParam;
	}

}	// anonymous namespace

	PlatformWindow::PlatformWindow() : _windowHandle( nullptr ), _threadHandle( reinterpret_cast<HANDLE>(_beginthreadex( nullptr, 4096u, &MessageThreadEntryPoint, &_windowHandle, 0, nullptr )) ) {}

// ---------------------------------------------------

	PlatformWindow::PlatformWindow( PlatformWindow&& window ) : _windowHandle( window._windowHandle ), _threadHandle( window._threadHandle ) {
		window._windowHandle	= nullptr;
		window._threadHandle	= nullptr;
	}

// ---------------------------------------------------

	PlatformWindow::~PlatformWindow() {
		if( _windowHandle ) {
		//	Send the custom close message to cause the window to shut down 'for real'.
			SendMessageW( _windowHandle, WM_CustomClose, 0, 0 );
		}

		if( _threadHandle ) {
			WaitForSingleObject( _threadHandle, INFINITE );
		}
	}

// ---------------------------------------------------

	WindowPresentationParameters PlatformWindow::GetPresentationParameters() const {
		RECT	clientRect;

		if( FALSE == GetClientRect( _windowHandle, &clientRect ) ) {
			return { _presentationMode, 0, 0 };
		}
		
		return { _presentationMode, static_cast<WindowDimension>( clientRect.right - clientRect.left ), static_cast<WindowDimension>( clientRect.bottom - clientRect.top ) };
	}

// ---------------------------------------------------

	void PlatformWindow::SetPresentationParameters( WindowPresentationParameters parameters ) {
		enum : DWORD {
			WindowedBehaviors	= SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER,
			FullscreenBehaviors	= SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW,

			WindowedStyle		= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,
			FullscreenStyle		= WS_POPUP
		};

	// ---

		const DWORD	effectiveStyle( WindowPresentationMode::Windowed == parameters.mode ? WindowedStyle : FullscreenStyle );

		RECT		clientAreaRect;

		clientAreaRect.left		= 0;
		clientAreaRect.top		= 0;
		clientAreaRect.right	= WindowPresentationMode::Windowed != parameters.mode ? GetSystemMetrics( SM_CXFULLSCREEN ) : parameters.widthInPixels;
		clientAreaRect.bottom	= WindowPresentationMode::Windowed != parameters.mode ? GetSystemMetrics( SM_CYFULLSCREEN ) : parameters.heightInPixels;

		AdjustWindowRect( &clientAreaRect, effectiveStyle, FALSE );

		SetWindowLongPtrW( _windowHandle, GWL_STYLE, effectiveStyle );
		SetWindowPos( _windowHandle,
					  WindowPresentationMode::Windowed == parameters.mode ? nullptr : HWND_TOPMOST,
					  0,
					  0,
					  clientAreaRect.right,
					  clientAreaRect.bottom,
					  WindowPresentationMode::Windowed == parameters.mode ? WindowedBehaviors : FullscreenBehaviors );
	}

// ---------------------------------------------------

	PlatformWindow::operator NativeWindowHandle() const {
		return _windowHandle;
	}

}	// namespace Win32
}	// namespace Eldritch2


