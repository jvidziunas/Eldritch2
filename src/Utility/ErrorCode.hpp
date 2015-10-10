/*==================================================================*\
  ErrorCode.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides a definition/standard for an error code datatype, used
  for pinpointing failures and providing additional semantic
  information about root cause without paying the (high) cost of the
  standard C++ exception model.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	namespace Errors {

		ENUM_CLASS( ::Eldritch2::int32 ) {
			OPERATION_NOT_SUPPORTED	= -9,
			ACCESS_DENIED			= -7,
			INVALID_OBJECT_STATE	= -6,
			BAD_FILE_NAME			= -5,
			NULL_PARAMETER			= -4,
			INVALID_PARAMETER		= -3,
			OUT_OF_MEMORY			= -2,
			UNSPECIFIED				= -1,
			NONE					=  0
		};

	}	// namespace Errors

// ---------------------------------------------------

	class ErrorCode {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this ErrorCode instance.
		ETForceInlineHint ErrorCode( ::Eldritch2::int32 bitfield = ::Eldritch2::Errors::NONE );
		// Constructs this ErrorCode instance.
		ETForceInlineHint ErrorCode( const ::Eldritch2::ErrorCode& src );

		// Destroys this ErrorCode instance.
		ETForceInlineHint ~ErrorCode();

	// - ERROR DETECTION ---------------------------------
		
		// Examines this ErrorCode to determine if it indicates a failure condition.
		ETForceInlineHint	operator bool() const;

	// - ASSIGNMENT --------------------------------------

		ETForceInlineHint const ::Eldritch2::ErrorCode&	operator=( const ::Eldritch2::ErrorCode& src );

	// - DEBUG/LOGGING INFORMATION -----------------------

		// Returns 
		ETForceInlineHint ::Eldritch2::int32	ToInt() const;

		// Retrieves a human-readable string describing this ErrorCode instance.
		const ::Eldritch2::UTF8Char*			ToUTF8String() const;

	// - DATA MEMBERS ------------------------------------
		
	private:
		::Eldritch2::int32	_value;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/ErrorCode.inl>
//------------------------------------------------------------------//