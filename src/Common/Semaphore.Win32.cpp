/*==================================================================*\
  Semaphore.Win32.cpp
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

Semaphore::Semaphore(uint32 maximumCount, uint32 initialCount) ETNoexceptHint : _semaphore(CreateSemaphoreW(nullptr, LONG(initialCount), LONG(maximumCount), nullptr)) {}

// ---------------------------------------------------

Semaphore::~Semaphore() {
	CloseHandle(_semaphore);
}

// ---------------------------------------------------

uint32 Semaphore::IncreaseCount(uint32 count) ETNoexceptHint {
	LONG previousCount(0);

	ReleaseSemaphore(_semaphore, static_cast<LONG>(count), &previousCount);

	return uint32(previousCount);
}

// ---------------------------------------------------

void Semaphore::Acquire() ETNoexceptHint {
	WaitForSingleObject(_semaphore, INFINITE);
}

// ---------------------------------------------------

bool Semaphore::TryAcquire() ETNoexceptHint {
	return WaitForSingleObject(_semaphore, 0u) == WAIT_OBJECT_0;
}

} // namespace Eldritch2
