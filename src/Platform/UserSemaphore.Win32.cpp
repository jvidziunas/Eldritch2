/*==================================================================*\
  UserSemaphore.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/UserSemaphore.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	UserSemaphore::UserSemaphore( uint32 maximumCount, uint32 initialCount ) : _semaphore( CreateSemaphoreW( nullptr, static_cast<LONG>( initialCount ), static_cast<LONG>( maximumCount ), nullptr ) ) {}

// ---------------------------------------------------

	UserSemaphore::~UserSemaphore() {
		CloseHandle( _semaphore );
	}

// ---------------------------------------------------

	uint32 UserSemaphore::IncreaseCount( uint32 count ) {
		LONG	previousCount( 0 );

		ReleaseSemaphore( _semaphore, static_cast<LONG>( count ), &previousCount );

		return static_cast<uint32>( previousCount );
	}

// ---------------------------------------------------

	void UserSemaphore::Acquire() {
		WaitForSingleObject( _semaphore, INFINITE );
	}

// ---------------------------------------------------

	bool UserSemaphore::TryAcquire() {
		return WAIT_OBJECT_0 == WaitForSingleObject( _semaphore, 0u );
	}

}	// namespace Platform
}	// namespace Eldritch2