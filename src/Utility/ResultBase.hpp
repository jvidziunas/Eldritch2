/*==================================================================*\
  ResultBase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename ResultPointer>
	class ResultBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		template <typename CompatibleResultPointer>
		ETForceInlineHint ResultBase( const Detail::ResultBase<CompatibleResultPointer>& result );
		ETForceInlineHint ResultBase( ResultPointer&& resultObject );
		ETForceInlineHint ResultBase( const ::Eldritch2::ErrorCode error );

	// ---------------------------------------------------

		ETForceInlineHint operator ::Eldritch2::ErrorCode() const;

		ETForceInlineHint operator bool() const;

	// ---------------------------------------------------

		ResultPointer			object;
		::Eldritch2::ErrorCode	resultCode;
	};

}	// namespace Detail
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/ResultBase.inl>
//------------------------------------------------------------------//

