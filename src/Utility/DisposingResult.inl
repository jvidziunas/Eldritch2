/*==================================================================*\
  DisposingResult.inl
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
namespace Utility {

	template <typename ResultObject>
	template <typename CompatibleResultObject>
	ETInlineHint DisposingResult<ResultObject>::DisposingResult( Scripting::ObjectHandle<CompatibleResultObject>&& object ) : object( ::std::move( object ) ), resultCode( ::Eldritch2::Error::NONE ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	template <typename CompatibleResultObject>
	ETInlineHint DisposingResult<ResultObject>::DisposingResult( Utility::DisposingResult<CompatibleResultObject>&& rhs ) : object( ::std::move( rhs.object ) ), resultCode( rhs.resultCode ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETInlineHint DisposingResult<ResultObject>::DisposingResult( const ::Eldritch2::ErrorCode errorCode ) : object( nullptr ), resultCode( errorCode ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint DisposingResult<ResultObject>::operator ::Eldritch2::ErrorCode&() {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint DisposingResult<ResultObject>::operator const ::Eldritch2::ErrorCode&() const {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint DisposingResult<ResultObject>::operator bool() const {
		return static_cast<bool>(resultCode);
	}

}	// namespace Utility
}	// namespace Eldritch2