/*==================================================================*\
  IntrusiveVyukovMPSCQueue.inl
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

	template <class StoredObject>
	typename IntrusiveVyukovMPSCQueue<StoredObject>::ValueType* IntrusiveVyukovMPSCQueue<StoredObject>::PopFront() {
		return static_cast<ValueType*>(IntrusiveVyukovMPSCQueueBase::PopFront());
	}

// ---------------------------------------------------

	template <class StoredObject>
	void IntrusiveVyukovMPSCQueue<StoredObject>::PushBack( ValueType& object ) {
		IntrusiveVyukovMPSCQueueBase::PushBack( object );
	}

}	// namespace Eldritch2