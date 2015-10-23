/*==================================================================*\
  Lock.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	class ETPureAbstractHint UserMutex : private Utility::Noncopyable {
	// ---------------------------------------------------

	public:
		//!	Acquires exclusive modification rights to access a shared resource.
		/*!	@remarks Blocks until rights are obtained. If aborting in the presence of contention is more appropriate, use @ref TryEnter() instead.
			@remarks The default implementation calls @ref TryEnter() in a busy loop.
			@see @ref Leave(), @ref TryEnter(), @ref ScopedLock
			*/
		virtual void	Enter();

		//!	Acquires exclusive modification rights to access a shared resource, in an interruptible fashion.
		/*!	@returns _True_ if exclusive write access was acquired, or _false_ if another thread currently has modification rights.
			@remarks Unlike @ref Enter(), the function will return immediately in the event another thread has write access.
			@see @ref Enter(), @ref Leave(), @ref ScopedLock
			*/
		virtual bool	TryEnter() abstract;

		//!	Releases exclusive modification rights to access a shared resource.
		/*!	@pre Modification rights should previously have been obtained for the caller via a call to one of @ref Enter() or @ref TryEnter().
			@see @ref Enter(), @ref TryEnter(), @ref ScopedLock
			*/
		virtual void	Leave() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref UserMutex instance.
		/*!	Designed to be called from within subclasses.
			*/
		UserMutex() = default;

	public:
		//!	Destroys this @ref UserMutex instance.
		/*!	Designed to be called from within subclasses.
			*/
		virtual ~UserMutex() = default;
	};

// ---------------------------------------------------

	class ETPureAbstractHint ReaderWriterUserMutex : public Utility::UserMutex {
	// ---------------------------------------------------

	public:
		//!	Acquires shared inspection rights for a shared resource.
		/*!	@remarks Blocks until rights are obtained. If aborting in the presence of contention is more appropriate, use @ref TryEnterAsReader() instead.
			@remarks The default implementation calls @ref TryEnterAsReader() in a busy loop.
			@remarks @parblock Multiple threads can all have read access to a resource, but only one thread may have write access at a time.
				Additionally, no threads may hold read access while any one thread has write access. @endparblock
			@see @ref LeaveAsReader(), @ref TryEnterAsReader(), @ref ScopedReaderLock
			*/
		virtual void	EnterAsReader();

		//!	Acquires shared inspection rights for a shared resource, in an interruptible fashion.
		/*!	@returns _True_ if read access was acquired, or _false_ if another thread currently has modification rights.
			@remarks Unlike @ref EnterAsReader(), the function will return immediately in the event another thread has write access.
			@see @ref EnterAsReader(), @ref LeaveAsReader(), @ref ScopedReaderLock
			*/
		virtual bool	TryEnterAsReader() abstract;

		//!	Releases shared inspection rights for a shared resource.
		/*!	@pre Read rights should previously have been obtained for the caller via a call to one of @ref EnterAsReader() or @ref TryEnterAsReader().
			@see @ref EnterAsReader(), @ref TryEnterAsReader() @ref ScopedReaderLock
			*/
		virtual void	LeaveAsReader() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref ReaderWriterUserMutex instance.
		/*!	Designed to be called from within subclasses.
			*/
		ReaderWriterUserMutex() = default;

	public:
		//! Destroys this @ref ReaderWriterUserMutex instance.
		/*!	Designed to be called from within subclasses.
			*/
		virtual ~ReaderWriterUserMutex() = default;
	};

// ---------------------------------------------------

	class ScopedLock : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScopedLock instance.
		ScopedLock( Utility::UserMutex& mutex );

		//!	Destroys this @ref ScopedLock instance.
		~ScopedLock();

	// ---------------------------------------------------

		bool	IsAttachedTo( const Utility::UserMutex& mutex ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::UserMutex&	_mutex;
	};

// ---------------------------------------------------

	class ScopedReaderLock : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ScopedReaderLock instance.
		ScopedReaderLock( Utility::ReaderWriterUserMutex& mutex );

		//!	Destroys this @ref ScopedReaderLock instance.
		~ScopedReaderLock();

	// ---------------------------------------------------

		bool	IsAttachedTo( const Utility::ReaderWriterUserMutex& mutex ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::ReaderWriterUserMutex&	_mutex;
	};

}	// namespace Utility
}	// namespace Eldritch2