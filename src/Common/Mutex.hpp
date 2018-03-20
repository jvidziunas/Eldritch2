/*==================================================================*\
  Mutex.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Platform.hpp>
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_WINDOWS
#	include <Windows.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETCacheLineAligned class Mutex {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Mutex instance.
		explicit Mutex( const Mutex& );
	//!	Constructs this @ref Mutex instance.
		Mutex();

		~Mutex();

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
		@see @ref Leave(), @ref TryEnter(), @ref ScopedLock */
		void	Enter();

	//!	Acquires exclusive modification rights to access a shared resource, in an interruptible fashion.
	/*!	@returns _True_ if exclusive write access was acquired, or _false_ if another thread currently has modification rights.
		@remarks Unlike @ref Enter(), the function will return immediately in the event another thread has write access.
		@see @ref Enter(), @ref Leave(), @ref ScopedLock */
		bool	TryEnter();

	//!	Releases exclusive modification rights to access a shared resource.
	/*!	@pre Modification rights should previously have been obtained for the caller via a call to one of @ref Enter() or @ref TryEnter().
		@see @ref Enter(), @ref TryEnter(), @ref ScopedLock */
		void	Leave();

	// ---------------------------------------------------

	public:
		Mutex&	operator=( const Mutex& );

	// - DATA MEMBERS ------------------------------------

	private:
#	if ET_PLATFORM_WINDOWS
		SRWLOCK	_lock;
#	endif
	};

// ---------------------------------------------------

	class Lock {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Lock( const Lock& ) = delete;
	//!	Constructs this @ref Lock instance.
		Lock( Mutex& mutex );

	//!	Destroys this @ref Lock instance.
		~Lock();

	// ---------------------------------------------------

	public:
		bool	IsAttachedTo( const Mutex& mutex ) const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Lock&	operator=( const Lock& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Mutex&	_mutex;
	};

// ---------------------------------------------------

	class ReadLock {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ReadLock( const ReadLock& ) = delete;
	//!	Constructs this @ref ReadLock instance.
		ReadLock( Mutex& mutex );

	//!	Destroys this @ref ReadLock instance.
		~ReadLock();

	// ---------------------------------------------------

	public:
		bool	IsAttachedTo( const Mutex& mutex ) const;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ReadLock&	operator=( const ReadLock& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Mutex&	_mutex;
	};

}	// namespace Eldritch2