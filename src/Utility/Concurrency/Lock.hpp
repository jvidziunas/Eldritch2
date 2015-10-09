/*==================================================================*\
  Lock.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint UserMutex : private Utility::Noncopyable {
	// ---------------------------------------------------

	public:
		virtual void	Enter() abstract;

		virtual void	Leave() abstract;

		virtual bool	TryEnter() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this UserMutex instance.
		UserMutex();

	public:
		// Destroys this UserMutex instance.
		virtual ~UserMutex();
	};

// ---------------------------------------------------

	class ETPureAbstractHint ReaderWriterUserMutex : public Utility::UserMutex {
	// ---------------------------------------------------

	public:
		virtual void	EnterAsReader() abstract;

		virtual void	LeaveAsReader() abstract;

		virtual bool	TryEnterAsReader() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this ReaderWriterUserMutex instance.
		ReaderWriterUserMutex();

	public:
		// Destroys this ReaderWriterUserMutex instance.
		virtual ~ReaderWriterUserMutex();
	};

// ---------------------------------------------------

	class ScopedLock : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this ScopedLock instance.
		ScopedLock( Utility::UserMutex* const mutex );
		// Constructs this ScopedLock instance.
		ScopedLock( Utility::UserMutex& mutex );

		// Destroys this ScopedLock instance.
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
		// Constructs this ScopedReaderLock instance.
		ScopedReaderLock( Utility::ReaderWriterUserMutex* const mutex );
		// Constructs this ScopedReaderLock instance.
		ScopedReaderLock( Utility::ReaderWriterUserMutex& mutex );

		// Destroys this ScopedReaderLock instance.
		~ScopedReaderLock();

	// ---------------------------------------------------

		bool	IsAttachedTo( const Utility::ReaderWriterUserMutex& mutex ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::ReaderWriterUserMutex&	_mutex;
	};

}	// namespace Utility
}	// namespace Eldritch2