/*==================================================================*\
  UserReadWriteMutex.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/UserReadWriteMutex.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	ScopedWriteLock::ScopedWriteLock( UserReadWriteMutex& lock ) : _mutex( lock ) {
		_mutex.Enter();
	}

// ---------------------------------------------------

	ScopedWriteLock::~ScopedWriteLock() {
		_mutex.Leave();
	}

// ---------------------------------------------------

	bool ScopedWriteLock::IsAttachedTo( const UserReadWriteMutex& mutex ) const {
		return &mutex == &_mutex;
	}

// ---------------------------------------------------

	ScopedReadLock::ScopedReadLock( UserReadWriteMutex& lock ) : _mutex( lock ) {
		_mutex.EnterAsReader();
	}

// ---------------------------------------------------

	ScopedReadLock::~ScopedReadLock() {
		_mutex.LeaveAsReader();
	}

// ---------------------------------------------------

	bool ScopedReadLock::IsAttachedTo( const UserReadWriteMutex& mutex ) const {
		return &mutex == &_mutex;
	}

}	// namespace Platform
}	// namespace Eldritch2