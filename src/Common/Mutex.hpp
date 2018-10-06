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
	explicit Mutex(const Mutex&) ETNoexceptHint;
	//!	Constructs this @ref Mutex instance.
	Mutex() ETNoexceptHint;

	~Mutex();

	// ---------------------------------------------------

public:
	//!	Acquires shared inspection rights for a shared resource, in an interruptible fashion.
	/*!	@returns _True_ if read access was acquired, or _false_ if another thread currently has modification rights.
		@remarks Unlike @ref EnterAsReader(), the function will return immediately in the event another thread has write access.
		@see @ref EnterAsReader(), @ref LeaveAsReader(), @ref ScopedReadLock */
	bool TryEnterAsReader() const ETNoexceptHint;

	//!	Releases shared inspection rights for a shared resource.
	/*!	@pre Read rights should previously have been obtained for the caller via a call to one of @ref EnterAsReader() or @ref TryEnterAsReader().
		@see @ref EnterAsReader(), @ref TryEnterAsReader() @ref ScopedReadLock */
	void LeaveAsReader() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	//!	Acquires exclusive modification rights to access a shared resource, in an interruptible fashion.
	/*!	@returns _True_ if exclusive write access was acquired, or _false_ if another thread currently has modification rights.
		@remarks Unlike @ref Enter(), the function will return immediately in the event another thread has write access.
		@see @ref Enter(), @ref Leave(), @ref ScopedLock */
	bool TryEnter() ETNoexceptHint;

	//!	Releases exclusive modification rights to access a shared resource.
	/*!	@pre Modification rights should previously have been obtained for the caller via a call to one of @ref Enter() or @ref TryEnter().
		@see @ref Enter(), @ref TryEnter(), @ref ScopedLock */
	void Leave() ETNoexceptHint;

	// ---------------------------------------------------

public:
	Mutex& operator=(const Mutex&) ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	mutable SRWLOCK _lock;
#endif
};

// ---

class Lock {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Lock instance.
	Lock(Mutex& mutex);
	//!	Disable copy construction.
	Lock(const Lock&) = delete;

	~Lock();

	// ---------------------------------------------------

public:
	bool IsAttachedTo(const Mutex& mutex) const ETNoexceptHint;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	Lock& operator=(const Lock&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	Mutex* _mutex;
};

// ---

class ReadLock {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref ReadLock instance.
	ReadLock(const Mutex& mutex);
	//!	Disable copy construction.
	ReadLock(const ReadLock&) = delete;

	~ReadLock();

	// ---------------------------------------------------

public:
	bool IsAttachedTo(const Mutex& mutex) const ETNoexceptHint;

	// ---------------------------------------------------

	//!	Disable copy assignment.
	ReadLock& operator=(const ReadLock&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
	const Mutex* _mutex;
};

} // namespace Eldritch2
