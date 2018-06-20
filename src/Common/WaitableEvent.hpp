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

class WaitableEvent {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum State {
		Unsignaled, //!< An outside source must manually invoke @ref Signal() on this @ref WaitableEvent instance before threads waiting on it will wake up.
		Signaled    //!< The first thread to block on this @ref WaitableEvent will immediately wake.
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref WaitableEvent instance.
	WaitableEvent(State initialState);

	~WaitableEvent();

	// ---------------------------------------------------

public:
	void AwaitSignal() const;

	bool IsSignaled() const;

	void Signal();

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	HANDLE _event;
#endif
};

} // namespace Eldritch2
