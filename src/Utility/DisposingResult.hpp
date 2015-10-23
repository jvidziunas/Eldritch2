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

	template <typename ResultObject>
	struct DisposingResult {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref DisposingResult instance.
		template <typename CompatibleResultObject>
		ETInlineHint DisposingResult( Scripting::ObjectHandle<CompatibleResultObject>&& object );
		//! Constructs this @ref DisposingResult instance.
		template <typename CompatibleResultObject>
		ETInlineHint DisposingResult( Utility::DisposingResult<CompatibleResultObject>&& rhs );
		//! Constructs this @ref DisposingResult instance.
		ETInlineHint explicit DisposingResult( const Utility::DisposingResult<ResultObject>& ) = default;
		//!	Constructs this @ref DisposingResult instance.
		ETInlineHint DisposingResult( const ::Eldritch2::ErrorCode errorCode );

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