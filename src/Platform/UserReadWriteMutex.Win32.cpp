/*==================================================================*\
  UserReadWriteMutex.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/UserReadWriteMutex.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	UserReadWriteMutex::UserReadWriteMutex( const UserReadWriteMutex& ) : UserReadWriteMutex() {}

// ---------------------------------------------------

	UserReadWriteMutex::UserReadWriteMutex() {
		InitializeSRWLock( &_lock );
	}

// ---------------------------------------------------

	UserReadWriteMutex::~UserReadWriteMutex() {}

// ---------------------------------------------------

	void UserReadWriteMutex::EnterAsReader() {
		AcquireSRWLockShared( &_lock );
	}

// ---------------------------------------------------

	bool UserReadWriteMutex::TryEnterAsReader() {
		return 0 != TryAcquireSRWLockShared( &_lock );
	}

// ---------------------------------------------------

	void UserReadWriteMutex::LeaveAsReader() {
		ReleaseSRWLockShared( &_lock );
	}

// ---------------------------------------------------

	void UserReadWriteMutex::Enter() {
		AcquireSRWLockExclusive( &_lock );
	}

// ---------------------------------------------------

	bool UserReadWriteMutex::TryEnter() {
		return 0 != TryAcquireSRWLockExclusive( &_lock );
	}

// ---------------------------------------------------

	void UserReadWriteMutex::Leave() {
		ReleaseSRWLockExclusive( &_lock );
	}

}	// namespace Platform
}	// namespace Eldritch2