/*==================================================================*\
  ReferenceCountable.inl
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
namespace Scripting {

	ETInlineHint void ReferenceCountable::AddReference() const {
		_referenceCount.fetch_add( 1u, ::std::memory_order_relaxed );
	}

// ---------------------------------------------------

	ETInlineHint void ReferenceCountable::ReleaseReference() const {
		if( 1u == _referenceCount.fetch_sub( 1u, ::std::memory_order_acq_rel ) ) {
			const_cast<ReferenceCountable*>(this)->Dispose();
		}
	}

// ---------------------------------------------------

	ETInlineHint size_t ReferenceCountable::GetReferenceCount() const {
		return _referenceCount.load( ::std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint ReferenceCountable::ReferenceCountable( const size_t initialReferenceCount ) : _referenceCount( initialReferenceCount ) {}

}	// namespace Scripting
}	// namespace Eldritch2