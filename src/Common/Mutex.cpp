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

Lock::Lock(Mutex& lock) :
	_mutex(lock) {
	while (!lock.TryEnter())
		;
}

// ---------------------------------------------------

Lock::~Lock() {
	_mutex.Leave();
}

// ---------------------------------------------------

bool Lock::IsAttachedTo(const Mutex& mutex) const {
	return &mutex == &_mutex;
}

// ---------------------------------------------------

ReadLock::ReadLock(const Mutex& lock) :
	_mutex(lock) {
	while (!lock.TryEnterAsReader())
		;
}

// ---------------------------------------------------

ReadLock::~ReadLock() {
	_mutex.LeaveAsReader();
}

// ---------------------------------------------------

bool ReadLock::IsAttachedTo(const Mutex& mutex) const {
	return &mutex == &_mutex;
}

} // namespace Eldritch2
