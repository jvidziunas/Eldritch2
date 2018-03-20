/*==================================================================*\
  Semaphore.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Semaphore.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	Semaphore::Semaphore( uint32 maximumCount, uint32 initialCount ) : _semaphore( CreateSemaphoreW( nullptr, static_cast<LONG>( initialCount ), static_cast<LONG>( maximumCount ), nullptr ) ) {}

// ---------------------------------------------------

	Semaphore::~Semaphore() {
		CloseHandle( _semaphore );
	}

// ---------------------------------------------------

	uint32 Semaphore::IncreaseCount( uint32 count ) {
		LONG	previousCount( 0 );

		ReleaseSemaphore( _semaphore, static_cast<LONG>( count ), &previousCount );

		return static_cast<uint32>( previousCount );
	}

// ---------------------------------------------------

	void Semaphore::Acquire() {
		WaitForSingleObject( _semaphore, INFINITE );
	}

// ---------------------------------------------------

	bool Semaphore::TryAcquire() {
		return WaitForSingleObject( _semaphore, 0u ) == WAIT_OBJECT_0;
	}

}	// namespace Eldritch2