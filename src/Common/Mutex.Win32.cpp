/*==================================================================*\
  Mutex.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mutex.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

Mutex::Mutex(const Mutex&) ETNoexceptHint : Mutex() {}

// ---------------------------------------------------

Mutex::Mutex() ETNoexceptHint {
	InitializeSRWLock(ETAddressOf(_lock));
}

// ---------------------------------------------------

Mutex::~Mutex() ETNoexceptHint {}

// ---------------------------------------------------

bool Mutex::TryEnterAsReader() const ETNoexceptHint {
	return TryAcquireSRWLockShared(ETAddressOf(_lock)) != 0;
}

// ---------------------------------------------------

void Mutex::LeaveAsReader() const ETNoexceptHint {
	ReleaseSRWLockShared(ETAddressOf(_lock));
}

// ---------------------------------------------------

bool Mutex::TryEnter() ETNoexceptHint {
	return TryAcquireSRWLockExclusive(ETAddressOf(_lock)) != 0;
}

// ---------------------------------------------------

void Mutex::Leave() ETNoexceptHint {
	ReleaseSRWLockExclusive(ETAddressOf(_lock));
}

// ---------------------------------------------------

Mutex& Mutex::operator=(const Mutex&) ETNoexceptHint {
	//	No-op.
	return *this;
}

} // namespace Eldritch2
