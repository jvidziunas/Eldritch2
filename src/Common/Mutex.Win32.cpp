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

	Mutex::Mutex( const Mutex& ) : Mutex() {}

// ---------------------------------------------------

	Mutex::Mutex() {
		InitializeSRWLock( &_lock );
	}

// ---------------------------------------------------

	Mutex::~Mutex() {}

// ---------------------------------------------------

	void Mutex::EnterAsReader() {
		AcquireSRWLockShared( &_lock );
	}

// ---------------------------------------------------

	bool Mutex::TryEnterAsReader() {
		return TryAcquireSRWLockShared( &_lock ) != 0;
	}

// ---------------------------------------------------

	void Mutex::LeaveAsReader() {
		ReleaseSRWLockShared( &_lock );
	}

// ---------------------------------------------------

	void Mutex::Enter() {
		AcquireSRWLockExclusive( &_lock );
	}

// ---------------------------------------------------

	bool Mutex::TryEnter() {
		return TryAcquireSRWLockExclusive( &_lock ) != 0;
	}

// ---------------------------------------------------

	void Mutex::Leave() {
		ReleaseSRWLockExclusive( &_lock );
	}

// ---------------------------------------------------

	Mutex& Mutex::operator =( const Mutex& ) {
	//	No-op.
		return *this;
	}

}	// namespace Eldritch2