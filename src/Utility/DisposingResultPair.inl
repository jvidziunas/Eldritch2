/*==================================================================*\
  DisposingResultPair.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename ResultObject>
	ETInlineHint DisposingResultPair<ResultObject>::DisposingResultPair( Scripting::ObjectHandle<ResultObject>&& object, const ::Eldritch2::ErrorCode resultCode ) : object( ::std::move( object ) ), resultCode( resultCode ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETInlineHint DisposingResultPair<ResultObject>::DisposingResultPair( Utility::DisposingResultPair<ResultObject>&& rhs ) : object( ::std::move( rhs.object ) ), resultCode( rhs.resultCode ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint DisposingResultPair<ResultObject>::operator ::Eldritch2::ErrorCode&() {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint DisposingResultPair<ResultObject>::operator const ::Eldritch2::ErrorCode&() const {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint DisposingResultPair<ResultObject>::operator bool() const {
		return static_cast<bool>(resultCode);
	}

}	// namespace Utility
}	// namespace Eldritch2