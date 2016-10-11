/*==================================================================*\
  IntrusiveMPSCQueue.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Value>
	IntrusiveMPSCQueue<Value>::IntrusiveMPSCQueue( IntrusiveMPSCQueue&& queue ) : Detail::IntrusiveMPSCQueueBase( eastl::move( queue ) ) {}

// ---------------------------------------------------

	template <class Value>
	typename IntrusiveMPSCQueue<Value>::ValueType* IntrusiveMPSCQueue<Value>::PopFront() {
		auto	result( static_cast<ValueType*>(IntrusiveMPSCQueueBase::PopFront()) );
#if( ET_DEBUG_MODE_ENABLED )
		if( result ) {
			result->mpNext = result;
		}
#endif
		return result;
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	bool IntrusiveMPSCQueue<Value>::PopFrontAndDispose( Disposer disposer ) {
		if( const auto element = PopFront() ) {
			disposer( *element );
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	template <class Value>
	void IntrusiveMPSCQueue<Value>::PushBack( ValueType& object ) {
		IntrusiveMPSCQueueBase::PushBack( object );
	}

// ---------------------------------------------------

	template <class Value>
	template <typename Disposer>
	void IntrusiveMPSCQueue<Value>::ClearAndDispose( Disposer disposer ) {
		while( PopFrontAndDispose( disposer ) );
	}

// ---------------------------------------------------

	template <class Value>
	void IntrusiveMPSCQueue<Value>::Clear() {
		ClearAndDispose( [] ( Value& ) {} );
	}

}	// namespace Eldritch2