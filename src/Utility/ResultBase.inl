/*==================================================================*\
  ResultBase.inl
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
namespace Detail {

	template <typename ResultPointer>
	template <typename CompatibleResultPointer>
	ETForceInlineHint ResultBase<ResultPointer>::ResultBase( const Detail::ResultBase<CompatibleResultPointer>& result ) : object( result.object ), resultCode( result.resultCode ) {}

// ---------------------------------------------------

	template <typename ResultPointer>
	ETForceInlineHint ResultBase<ResultPointer>::ResultBase( ResultPointer&& resultObject ) : object( ::std::move( resultObject ) ), resultCode( ::Eldritch2::Error::None ) {}

// ---------------------------------------------------

	template <typename ResultPointer>
	ETForceInlineHint ResultBase<ResultPointer>::ResultBase( const ::Eldritch2::ErrorCode error ) : object(), resultCode( error ) {}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint ResultBase<ResultObject>::operator ::Eldritch2::ErrorCode() const {
		return resultCode;
	}

// ---------------------------------------------------

	template <typename ResultObject>
	ETForceInlineHint ResultBase<ResultObject>::operator bool() const {
		return static_cast<bool>(resultCode);
	}

}	// namespace Detail
}	// namespace Eldritch2

