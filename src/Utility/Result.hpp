/*==================================================================*\
  Result.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	template <typename ResultObject>
	struct Result {
		template <typename CompatibleResultObject>
		ETForceInlineHint Result( const Utility::Result<CompatibleResultObject>& result );
		template <typename CompatibleResultObject>
		ETForceInlineHint Result( CompatibleResultObject& object );
		ETForceInlineHint Result( const ::Eldritch2::ErrorCode error );

		ETForceInlineHint operator ::Eldritch2::ErrorCode&();
		ETForceInlineHint operator const ::Eldritch2::ErrorCode&() const;

		ETForceInlineHint operator bool() const;

	// ---------------------------------------------------

		ResultObject*			object;
		::Eldritch2::ErrorCode	resultCode;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Result.inl>
//------------------------------------------------------------------//