/*==================================================================*\
  WaitableEvent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {

enum class SignalState {
	Unsignaled, //!< An outside source must manually invoke @ref Signal() on this @ref WaitableEvent instance before threads waiting on it will wake up.
	Signaled    //!< The first thread to block on this @ref WaitableEvent will immediately wake.
};

// ---

class WaitableEvent {
	// - TYPE PUBLISHING ---------------------------------

public:
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref WaitableEvent instance.
	WaitableEvent(SignalState initialState) ETNoexceptHint;
	//!	Disable copy construction.
	WaitableEvent(const WaitableEvent&) = delete;

	~WaitableEvent();

	// ---------------------------------------------------

public:
	void AwaitSignal() const ETNoexceptHint;

	bool IsSignaled() const ETNoexceptHint;

	void Signal() ETNoexceptHint;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	WaitableEvent& operator=(const WaitableEvent&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	HANDLE _event;
#endif
};

} // namespace Eldritch2
