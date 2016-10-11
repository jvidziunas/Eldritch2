/*==================================================================*\
  PlatformWindow.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/Mpl/Platform.hpp>
//------------------------------------------------------------------//

#if ET_PLATFORM_WINDOWS
struct	HWND__;

using HWND		= HWND__*;
using HANDLE	= void*;
#endif

namespace Eldritch2 {
namespace Platform {

#if ET_PLATFORM_WINDOWS
	using WindowDimension		= Eldritch2::uint16;
	using NativeWindowHandle	= HWND;
#endif

// ---

	enum class WindowPresentationMode {
		Windowed,
		WindowedFullscreen,
		Fullscreen
	};

// ---

	struct WindowPresentationParameters {
		WindowPresentationMode	mode;
		WindowDimension			widthInPixels;
		WindowDimension			heightInPixels;
	};

// ---

	class PlatformWindow {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copying.
		PlatformWindow( const PlatformWindow& ) = delete;
	//!	Constructs this @ref PlatformWindow instance.
		PlatformWindow( PlatformWindow&& );
	//!	Constructs this @ref PlatformWindow instance.
		PlatformWindow();

		~PlatformWindow();

	// ---------------------------------------------------

	public:
		WindowPresentationParameters	GetPresentationParameters() const;

		void							SetPresentationParameters( WindowPresentationParameters parameters );

	// ---------------------------------------------------

	public:
		explicit operator NativeWindowHandle() const;

	// ---------------------------------------------------

	//!	Disable assignment.
		PlatformWindow&	operator=( const PlatformWindow& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	#if ET_PLATFORM_WINDOWS
		WindowPresentationMode	_presentationMode;
		NativeWindowHandle		_windowHandle;
		HANDLE					_threadHandle;
	#endif
	};

}	// namespace Platform
}	// namespace Eldritch2