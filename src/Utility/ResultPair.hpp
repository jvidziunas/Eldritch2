/*==================================================================*\
  ResultPair.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename ResultObject>
	struct ResultPair {
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
#include <Utility/ResultPair.inl>
//------------------------------------------------------------------//