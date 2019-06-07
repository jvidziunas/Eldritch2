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
#include <Common/Atomic.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
enum class Result : int;
} // namespace Eldritch2

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
	Window(Window&&) ETNoexceptHint;
	//!	Constructs this @ref Window instance.
	Window() ETNoexceptHint;

	~Window();

	// ---------------------------------------------------

public:
	DisplayMode GetDisplayMode() const;

	void SetDisplayMode(DisplayMode mode);

	// ---------------------------------------------------

#if ET_PLATFORM_WINDOWS
public:
	HINSTANCE GetHinstance() const ETNoexceptHint;

	HWND GetHwnd() const ETNoexceptHint;
#endif

	// ---------------------------------------------------

public:
	void EnsureVisible();

	bool Exists() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Result BindResources();

	void FreeResources();

	// ---------------------------------------------------

	//!	Disable copy assignment.
	Window& operator=(const Window&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	FullscreenMode _mode;
	HWND           _window;
#endif

	// ---------------------------------------------------

	friend void Swap(Window&, Window&) ETNoexceptHint;
};

bool IsForegroundApplication(MemoryOrder order = std::memory_order_acq_rel);

Result InitializeUiThread();

void TerminateUiThread();

} // namespace Eldritch2
