/*==================================================================*\
  Mutex.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mutex.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

Lock::Lock(Mutex& mutex) :
	_mutex(ETAddressOf(mutex)) {
	while (mutex.TryEnter() == false) {
		// Busy wait.
	}
}

// ---------------------------------------------------

Lock::~Lock() {
	_mutex->Leave();
}

// ---------------------------------------------------

bool Lock::IsAttachedTo(const Mutex& mutex) const ETNoexceptHint {
	return ETAddressOf(mutex) == _mutex;
}

// ---------------------------------------------------

ReadLock::ReadLock(const Mutex& mutex) :
	_mutex(ETAddressOf(mutex)) {
	while (mutex.TryEnterAsReader() == false) {
		// Busy wait.
	}
}

// ---------------------------------------------------

ReadLock::~ReadLock() {
	_mutex->LeaveAsReader();
}

// ---------------------------------------------------

bool ReadLock::IsAttachedTo(const Mutex& mutex) const ETNoexceptHint {
	return ETAddressOf(mutex) == _mutex;
}

} // namespace Eldritch2
