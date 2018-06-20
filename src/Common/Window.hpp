/*==================================================================*\
  Window.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class ErrorCode;
}

#if ET_PLATFORM_WINDOWS
using HINSTANCE = struct HINSTANCE__*;
using HWND      = struct HWND__*;
using HANDLE    = void*;
#endif

namespace Eldritch2 {

enum class FullscreenMode {
	Windowed,
	WindowedFullscreen,
	Fullscreen
};

// ---

struct DisplayMode {
	FullscreenMode mode;
	uint16         widthInPixels;
	uint16         heightInPixels;
};

// ---

class Window {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	Window(const Window&) = delete;
	//!	Constructs this @ref Window instance.
	Window(Window&&);
	//!	Constructs this @ref Window instance.
	Window();

	~Window();

	// ---------------------------------------------------

public:
	DisplayMode GetDisplayMode() const;

	void SetDisplayMode(DisplayMode mode);

	// ---------------------------------------------------

public:
#if ET_PLATFORM_WINDOWS
	HINSTANCE GetHinstance() const;

	HWND GetHwnd() const;
#endif

	// ---------------------------------------------------

public:
	void EnsureVisible();

	bool Exists() const;

	// ---------------------------------------------------

public:
	ErrorCode BindResources();

	void FreeResources();

	// ---------------------------------------------------

	//!	Disable copy assignment.
	Window& operator=(const Window&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	FullscreenMode _mode;
	HWND           _window;
	HANDLE         _messageThread;
#endif

	// ---------------------------------------------------

	friend void Swap(Window&, Window&);
};

} // namespace Eldritch2
