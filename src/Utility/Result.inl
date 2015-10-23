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
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename ResultObject>
	template <typename CompatibleResultObject>
	ETForceInlineHint Result<ResultObject>::Result( const Utility::Result<CompatibleResultObject>& result ) : object( static_cast<ResultObject*>(result.object) ), resultCode( result.resultCode ) {
		static_assert( ::std::is_convertible<CompatibleResultObject*, ResultObject*>::value, "Result types must be constructed from compatible types!" );
	}

// ---------------------------------------------------

	template <typename ResultObject>
	template <typename CompatibleResultObject>
	ETForceInlineHint Result<ResultObject>::Result( CompatibleResultObject& object ) : object( &object ), resultCode( ::Eldritch2::Error::NONE ) {
		static_assert( ::std::is_convertible<CompatibleResultObject*, ResultObject*>::value, "Result types must be constructed from compatible types!" );
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint Result<ResultObject>::Result( const ::Eldritch2::ErrorCode error ) : object( nullptr ), resultCode( error ) {}

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