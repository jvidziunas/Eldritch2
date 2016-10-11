/*==================================================================*\
  IntrusiveMPSCQueue.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	class IntrusiveMPSCQueueBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref IntrusiveMPSCQueueBase instance.
		IntrusiveMPSCQueueBase( const IntrusiveMPSCQueueBase& ) = delete;
	//!	Constructs this @ref IntrusiveMPSCQueueBase instance.
	/*!	Designed to be called from within subclasses. */
		IntrusiveMPSCQueueBase( IntrusiveMPSCQueueBase&& );
	//!	Constructs this @ref IntrusiveMPSCQueueBase instance.
	/*!	Designed to be called from within subclasses. */
		IntrusiveMPSCQueueBase();

	//!	Destroys this @ref IntrusiveMPSCQueueBase instance.
	/*!	Designed to be called from within subclasses. */
		~IntrusiveMPSCQueueBase() = default;

	// ---------------------------------------------------

		Eldritch2::IntrusiveForwardListBaseHook*	PopFront();

		void										PushBack( Eldritch2::IntrusiveForwardListBaseHook& object );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::IntrusiveForwardListBaseHook										_stub;

		ETCacheLineAligned std::atomic<Eldritch2::IntrusiveForwardListBaseHook*>	_head;
		ETCacheLineAligned std::atomic<Eldritch2::IntrusiveForwardListBaseHook*>	_tail;
	};

}	// namespace Detail

	template <class Value>
	class IntrusiveMPSCQueue : private Detail::IntrusiveMPSCQueueBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType	= Value;
		using Reference = ValueType&;
		using Pointer	= ValueType*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref IntrusiveMPSCQueue instance.
		IntrusiveMPSCQueue( const IntrusiveMPSCQueue& ) = delete;
	//!	Constructs this @ref IntrusiveMPSCQueue instance.
		IntrusiveMPSCQueue( IntrusiveMPSCQueue&& );
	//!	Constructs this @ref IntrusiveMPSCQueue instance.
		IntrusiveMPSCQueue() = default;

		~IntrusiveMPSCQueue() = default;

	// ---------------------------------------------------

	public:
	//!	Removes the head element from the queue.
	/*!	@returns A pointer to the removed element if the queue is not empty, or a null pointer if the queue was empty.
		@remarks Thread-safe. */
		ValueType*	PopFront();

		template <typename Disposer>
		bool		PopFrontAndDispose( Disposer disposer );

		void		PushBack( ValueType& object );

	// ---------------------------------------------------

		template <typename Disposer>
		void	ClearAndDispose( Disposer disposer );

		void	Clear();
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/IntrusiveMPSCQueue.inl>
//------------------------------------------------------------------//