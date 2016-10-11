/*==================================================================*\
  UserReadWriteMutex.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Platform.hpp>
#include <Utility/MPL/Compiler.hpp>
#if ET_PLATFORM_WINDOWS
#	include <Windows.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Platform {

	ETCacheLineAligned class UserReadWriteMutex {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UserReadWriteMutex instance.
		explicit UserReadWriteMutex( const UserReadWriteMutex& );
	//!	Constructs this @ref UserReadWriteMutex instance.
		UserReadWriteMutex();

		~UserReadWriteMutex();

	// ---------------------------------------------------

	public:
	//!	Acquires shared inspection rights for a shared resource.
	/*!	@remarks Blocks until rights are obtained. If aborting in the presence of contention is more appropriate, use @ref TryEnterAsReader() instead.
		@remarks The default implementation calls @ref TryEnterAsReader() in a busy loop.
		@remarks @parblock Multiple threads can all have read access to a resource, but only one thread may have write access at a time.
			Additionally, no threads may hold read access while any one thread has write access. @endparblock
		@see @ref LeaveAsReader(), @ref TryEnterAsReader(), @ref ScopedReadLock */
		void	EnterAsReader();

	//!	Acquires shared inspection rights for a shared resource, in an interruptible fashion.
	/*!	@returns _True_ if read access was acquired, or _false_ if another thread currently has modification rights.
		@remarks Unlike @ref EnterAsReader(), the function will return immediately in the event another thread has write access.
		@see @ref EnterAsReader(), @ref LeaveAsReader(), @ref ScopedReadLock */
		bool	TryEnterAsReader();

	//!	Releases shared inspection rights for a shared resource.
	/*!	@pre Read rights should previously have been obtained for the caller via a call to one of @ref EnterAsReader() or @ref TryEnterAsReader().
		@see @ref EnterAsReader(), @ref TryEnterAsReader() @ref ScopedReadLock */
		void	LeaveAsReader();

	// ---------------------------------------------------

	public:
	//!	Acquires exclusive modification rights to access a shared resource.
	/*!	@remarks Blocks until rights are obtained. If aborting in the presence of contention is more appropriate, use @ref TryEnter() instead.
		@remarks The default implementation calls @ref TryEnter() in a busy loop.
		@see @ref Leave(), @ref TryEnter(), @ref ScopedWriteLock */
		void	Enter();

	//!	Acquires exclusive modification rights to access a shared resource, in an interruptible fashion.
	/*!	@returns _True_ if exclusive write access was acquired, or _false_ if another thread currently has modification rights.
		@remarks Unlike @ref Enter(), the function will return immediately in the event another thread has write access.
		@see @ref Enter(), @ref Leave(), @ref ScopedWriteLock */
		bool	TryEnter();

	//!	Releases exclusive modification rights to access a shared resource.
	/*!	@pre Modification rights should previously have been obtained for the caller via a call to one of @ref Enter() or @ref TryEnter().
		@see @ref Enter(), @ref TryEnter(), @ref ScopedWriteLock */
		void	Leave();

	// ---------------------------------------------------

	private:
		UserReadWriteMutex&	operator=( const UserReadWriteMutex& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
#	if ET_PLATFORM_WINDOWS
		SRWLOCK	_lock;
#	endif
	};

// ---------------------------------------------------

	class ScopedWriteLock {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ScopedWriteLock instance.
		ScopedWriteLock( Platform::UserReadWriteMutex& mutex );
	//!	Constructs this @ref ScopedWriteLock instance.
		ScopedWriteLock( const ScopedWriteLock& ) = delete;

	//!	Destroys this @ref ScopedWriteLock instance.
		~ScopedWriteLock();

	// ---------------------------------------------------

	public:
		bool	IsAttachedTo( const Platform::UserReadWriteMutex& mutex ) const;

	// ---------------------------------------------------

		ScopedWriteLock&	operator=( const ScopedWriteLock& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Platform::UserReadWriteMutex&	_mutex;
	};

// ---------------------------------------------------

	class ScopedReadLock {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ScopedReadLock instance.
		ScopedReadLock( Platform::UserReadWriteMutex& mutex );
	//!	Constructs this @ref ScopedReadLock instance.
		ScopedReadLock( const ScopedReadLock& ) = delete;

	//!	Destroys this @ref ScopedReadLock instance.
		~ScopedReadLock();

	// ---------------------------------------------------

	public:
		bool	IsAttachedTo( const Platform::UserReadWriteMutex& mutex ) const;

	// ---------------------------------------------------

		ScopedReadLock&	operator=( const ScopedReadLock& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Platform::UserReadWriteMutex&	_mutex;
	};

}	// namespace Platform
}	// namespace Eldritch2