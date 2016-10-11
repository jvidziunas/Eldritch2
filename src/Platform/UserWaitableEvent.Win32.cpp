/*==================================================================*\
  UserWaitableEvent.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/UserWaitableEvent.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	UserWaitableEvent::UserWaitableEvent( State initialState ) : _event( CreateEventW( nullptr, FALSE, (initialState == Signaled ? TRUE : FALSE), nullptr ) ) {}

// ---------------------------------------------------

	UserWaitableEvent::~UserWaitableEvent() {
		CloseHandle( _event );
	}

// ---------------------------------------------------

	void UserWaitableEvent::AwaitSignal() const {
		WaitForSingleObject( _event, INFINITE );
	}

// ---------------------------------------------------

	bool UserWaitableEvent::IsSignaled() const {
		return WAIT_OBJECT_0 == WaitForSingleObject( _event, 0u );
	}

// ---------------------------------------------------

	void UserWaitableEvent::Signal() {
		SetEvent( _event );
	}

}	// namespace Platform
}	// namespace Win32