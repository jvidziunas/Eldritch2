/*==================================================================*\
  IntrusiveMPSCQueue.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveMPSCQueue.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {
namespace {

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T AtomicLoad( T& value, std::memory_order order ) {
		return std::atomic_load_explicit( reinterpret_cast<std::atomic<T>*>(&value), order );
	}

	template <typename T>
	ETForceInlineHint ETPureFunctionHint void AtomicStore( T& value, T desiredValue, std::memory_order order ) {
		return std::atomic_store_explicit( reinterpret_cast<std::atomic<T>*>(&value), desiredValue, order );
	}

}	// anonymous namespace

	IntrusiveMPSCQueueBase::IntrusiveMPSCQueueBase( IntrusiveMPSCQueueBase&& queue ) {
		while( auto node = queue.PopFront() ) {
			PushBack( *node );
		}
	}

// ---------------------------------------------------

	IntrusiveMPSCQueueBase::IntrusiveMPSCQueueBase() : _head( &_stub ), _tail( &_stub ) {
		_stub.mpNext = nullptr;
	}

// ---------------------------------------------------

	Eldritch2::IntrusiveForwardListBaseHook* IntrusiveMPSCQueueBase::PopFront() {
		auto	tail( _tail.load( std::memory_order_acquire ) );
		auto	next( AtomicLoad( tail->mpNext, std::memory_order_consume ) );
		
		if( tail == &_stub ) {
			if( next == nullptr ) {
				return nullptr;
			}
				
			_tail.store( next, std::memory_order_release );
			tail = next;
			next = AtomicLoad( next->mpNext, std::memory_order_acquire );
		}

		if( next ) {
			_tail.store( next, std::memory_order_release );
			return tail;
		}

		if( tail != _head.load( std::memory_order_acquire ) ) {
			return nullptr;
		}
			
		PushBack( _stub );
		next = AtomicLoad( tail->mpNext, std::memory_order_acquire );
		if( next ) {
			_tail.store( next, std::memory_order_release );
			return tail;
		}

		return nullptr;
	}

// ---------------------------------------------------

	void IntrusiveMPSCQueueBase::PushBack( Eldritch2::IntrusiveForwardListBaseHook& object ) {
		AtomicStore( object.mpNext, static_cast<Eldritch2::IntrusiveForwardListBaseHook*>( nullptr ), std::memory_order_release );

		AtomicStore( _head.exchange( &object, std::memory_order_release )->mpNext, &object, std::memory_order_release );
	}

}	// namespace Detail
}	// namespace Eldritch2
