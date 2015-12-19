/*==================================================================*\
  CountedResult.inl
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
	ETInlineHint CountedResult<ResultObject>::CountedResult( Scripting::ObjectHandle<CompatibleResultObject>&& object ) : object( ::std::move( object ) ), resultCode( ::Eldritch2::Error::NONE ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	template <typename CompatibleResultObject>
	ETInlineHint CountedResult<ResultObject>::CountedResult( Utility::CountedResult<CompatibleResultObject>&& rhs ) : object( ::std::move( rhs.object ) ), resultCode( rhs.resultCode ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETInlineHint CountedResult<ResultObject>::CountedResult( const ::Eldritch2::ErrorCode errorCode ) : object( nullptr ), resultCode( errorCode ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint CountedResult<ResultObject>::operator ::Eldritch2::ErrorCode&() {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint CountedResult<ResultObject>::operator const ::Eldritch2::ErrorCode&() const {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint CountedResult<ResultObject>::operator bool() const {
		return static_cast<bool>(resultCode);
	}

}	// namespace Utility
}	// namespace Eldritch2