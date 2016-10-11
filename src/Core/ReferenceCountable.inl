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
namespace Core {

	ETInlineHint ReferenceCountable::ReferenceCountable() : _referenceCount( 1u ) {}

// ---------------------------------------------------

	ETInlineHint ReferenceCountable::ReferenceCountable( const ReferenceCountable& /*unused*/ ) : ReferenceCountable() {}

// ---------------------------------------------------

	ETInlineHint size_t ReferenceCountable::GetReferenceCount() const {
		return _referenceCount.load( std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint size_t ReferenceCountable::AddReference() {
		return _referenceCount.fetch_add( 1u, std::memory_order_relaxed );
	}

// ---------------------------------------------------

	ETInlineHint size_t ReferenceCountable::ReleaseReference() {
		return _referenceCount.fetch_sub( 1u, std::memory_order_acq_rel );
	}

// ---------------------------------------------------

	ETInlineHint ReferenceCountable& ReferenceCountable::operator=( const ReferenceCountable& /*unused*/ ) {
	//	Intentionally a no-op.
		return *this;
	}

}	// namespace Core
}	// namespace Eldritch2