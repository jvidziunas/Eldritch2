/*==================================================================*\
  Lock.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Concurrency/Lock.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

	UserMutex::UserMutex() {}

// ---------------------------------------------------

	UserMutex::~UserMutex() {}

// ---------------------------------------------------

	ReaderWriterUserMutex::ReaderWriterUserMutex() {}

// ---------------------------------------------------

	ReaderWriterUserMutex::~ReaderWriterUserMutex() {}

// ---------------------------------------------------

	ScopedLock::ScopedLock( UserMutex* const lock ) : _mutex( *lock ) {
		_mutex.Enter();
	}

// ---------------------------------------------------

	ScopedLock::ScopedLock( UserMutex& lock ) : _mutex( lock ) {
		_mutex.Enter();
	}

// ---------------------------------------------------

	ScopedLock::~ScopedLock() {
		_mutex.Leave();
	}

// ---------------------------------------------------

	bool ScopedLock::IsAttachedTo( const Utility::UserMutex& lock ) const {
		return &lock == &_mutex;
	}

// ---------------------------------------------------

	ScopedReaderLock::ScopedReaderLock( ReaderWriterUserMutex* const lock ) : _mutex( *lock ) {
		_mutex.EnterAsReader();
	}

// ---------------------------------------------------

	ScopedReaderLock::ScopedReaderLock( ReaderWriterUserMutex& lock ) : _mutex( lock ) {
		_mutex.EnterAsReader();
	}

// ---------------------------------------------------

	ScopedReaderLock::~ScopedReaderLock() {
		_mutex.LeaveAsReader();
	}

// ---------------------------------------------------

	bool ScopedReaderLock::IsAttachedTo( const ReaderWriterUserMutex& lock ) const {
		return &lock == &_mutex;
	}

}	// namespace Utility
}	// namespace Eldritch2