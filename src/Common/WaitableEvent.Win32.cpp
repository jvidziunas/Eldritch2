/*==================================================================*\
  WaitableEvent.Win32.cpp
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
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

WaitableEvent::WaitableEvent(SignalState initialState) ETNoexceptHint : _event(CreateEventW(/*lpEventAttributes =*/nullptr, /*bManualReset =*/FALSE, (initialState == SignalState::Signaled ? TRUE : FALSE), /*lpName =*/nullptr)) {}

// ---------------------------------------------------

WaitableEvent::~WaitableEvent() {
	CloseHandle(_event);
}

// ---------------------------------------------------

void WaitableEvent::AwaitSignal() const ETNoexceptHint {
	WaitForSingleObject(_event, /*dwMilliseconds =*/INFINITE);
}

// ---------------------------------------------------

bool WaitableEvent::IsSignaled() const ETNoexceptHint {
	return WaitForSingleObject(_event, /*dwMilliseconds =*/0u) == WAIT_OBJECT_0;
}

// ---------------------------------------------------

void WaitableEvent::Signal() ETNoexceptHint {
	SetEvent(_event);
}

} // namespace Eldritch2
