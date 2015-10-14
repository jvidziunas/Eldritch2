/*==================================================================*\
  DisposingResult.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ObjectHandle.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <typename ResultObject>
	struct DisposingResult {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref DisposingResult instance.
		ETInlineHint DisposingResult( Scripting::ObjectHandle<ResultObject>&& object );
		//!	Constructs this @ref DisposingResult instance.
		ETInlineHint DisposingResult( const ::Eldritch2::ErrorCode errorCode );
		//! Constructs this @ref DisposingResult instance.
		ETInlineHint DisposingResult( Utility::DisposingResult<ResultObject>&& rhs );

	// ---------------------------------------------------

		ETForceInlineHint operator ::Eldritch2::ErrorCode&();
		ETForceInlineHint operator const ::Eldritch2::ErrorCode&() const;

		ETForceInlineHint operator bool() const;

	// ---------------------------------------------------

		Scripting::ObjectHandle<ResultObject>	object;
		::Eldritch2::ErrorCode					resultCode;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/DisposingResult.inl>
//------------------------------------------------------------------//