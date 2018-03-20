/*==================================================================*\
  WaitableEvent.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/WaitableEvent.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	WaitableEvent::WaitableEvent( State initialState ) : _event( CreateEventW( nullptr, FALSE, (initialState == Signaled ? TRUE : FALSE), nullptr ) ) {}

// ---------------------------------------------------

	WaitableEvent::~WaitableEvent() {
		CloseHandle( _event );
	}

// ---------------------------------------------------

	void WaitableEvent::AwaitSignal() const {
		WaitForSingleObject( _event, INFINITE );
	}

// ---------------------------------------------------

	bool WaitableEvent::IsSignaled() const {
		return WaitForSingleObject( _event, 0u ) == WAIT_OBJECT_0;
	}

// ---------------------------------------------------

	void WaitableEvent::Signal() {
		SetEvent( _event );
	}

}	// namespace Win32