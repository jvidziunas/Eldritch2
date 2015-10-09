/*==================================================================*\
  IntrusiveVyukovMPSCQueue.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.hpp>
//------------------------------------------------------------------//

using namespace ::rde;
using namespace ::std;

namespace {

	template <typename T>
	ETForceInlineHint ETNoAliasHint T AtomicLoad( T& value, memory_order order ) {
		return atomic_load_explicit( reinterpret_cast<atomic<T>*>(&value), order );
	}

}	// anonymous namespace

namespace Eldritch2Detail {

	IntrusiveVyukovMPSCQueueBase::IntrusiveVyukovMPSCQueueBase() : _head( &_stub ), _tail( &_stub ) {
		_stub.next = nullptr;
	}

// ---------------------------------------------------

	intrusive_slist_node* IntrusiveVyukovMPSCQueueBase::PopFront() {
		auto*	tail( _tail.load( memory_order_acquire ) );
		auto*	next( AtomicLoad( tail->next, memory_order_consume ) );
		
		if( tail == &_stub ) {
			if( next == nullptr ) {
				return nullptr;
			}
				
			_tail.store( next, memory_order_release );
			tail = next;
			next = AtomicLoad( next->next, memory_order_acquire );
		}

		if( next ) {
			_tail.store( next, memory_order_release );
			return tail;
		}

		if( tail != _head.load( memory_order_acquire ) ) {
			return nullptr;
		}
			
		PushBack( _stub );
		next = AtomicLoad( tail->next, memory_order_acquire );
		if( next ) {
			_tail.store( next, memory_order_release );
			return tail;
		}

		return nullptr;
	}

// ---------------------------------------------------

	void IntrusiveVyukovMPSCQueueBase::PushBack( intrusive_slist_node& object ) {
		atomic_store_explicit( reinterpret_cast<atomic<decltype(&object)>*>(&object.next), nullptr, memory_order_release );

		auto* const	prev( _head.exchange( &object, memory_order_release ) );

		atomic_store_explicit( reinterpret_cast<atomic<decltype(&object)>*>(prev->next), &object, memory_order_release );
	}

}	// namespace Eldritch2Detail



