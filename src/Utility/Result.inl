/*==================================================================*\
  Result.inl
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

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint Result<ResultObject>::Result( const ::Eldritch2::ErrorCode error ) : object( nullptr ), resultCode( error ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint Result<ResultObject>::Result( ResultObject& object ) : object( &object ), resultCode( ::Eldritch2::Error::NONE ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint Result<ResultObject>::operator ::Eldritch2::ErrorCode&() {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint Result<ResultObject>::operator const ::Eldritch2::ErrorCode&() const {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint Result<ResultObject>::operator bool() const {
		return static_cast<bool>( resultCode );
	}

}	// namespace Utility
}	// namespace Eldritch2