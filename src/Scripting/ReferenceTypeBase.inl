/*==================================================================*\
  ReferenceTypeBase.inl
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

	ETInlineHint void ReferenceTypeBase::AddReference() {
		_referenceCount.fetch_add( 1u, ::std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint void ReferenceTypeBase::ReleaseReference() {
		if( 1u == _referenceCount.fetch_sub( 1u, ::std::memory_order_release ) ) {
			this->Dispose();
		}
	}

// ---------------------------------------------------

	ETInlineHint size_t ReferenceTypeBase::GetReferenceCount() const {
		return _referenceCount.load( ::std::memory_order_consume );
	}

// ---------------------------------------------------

	ETInlineHint ReferenceTypeBase::ReferenceTypeBase( const size_t initialReferenceCount ) : _referenceCount( initialReferenceCount ) {}

}	// namespace Scripting
}	// namespace Eldritch2