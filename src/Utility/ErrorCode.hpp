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

	enum class Error : ::Eldritch2::int32 {
		UnsupportedOperation	= -9,
		AccessDenied			= -7,
		InvalidObjectState		= -6,
		InvalidFileName			= -5,
		NullParameter			= -4,
		InvalidParameter		= -3,
		OutOfMemory				= -2,
		Unspecified				= -1,
		None					=  0
	};

// ---------------------------------------------------

	class ErrorCode {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ErrorCode instance.
		ETForceInlineHint ErrorCode( const ::Eldritch2::Error bitfield = ::Eldritch2::Error::None );
		//!	Constructs this @ref ErrorCode instance.
		ETForceInlineHint ErrorCode( const ::Eldritch2::ErrorCode& source ) = default;

		//!	Destroys this @ref ErrorCode instance.
		ETForceInlineHint ~ErrorCode() = default;

	// - ERROR DETECTION ---------------------------------
		
		//!	Examines this @ref ErrorCode to determine if it indicates a failure condition.
		ETForceInlineHint	operator bool() const;

	// - ASSIGNMENT --------------------------------------

		//!	Copies an @ref ErrorCode.
		/*!	@param[in] source The @ref ErrorCode to copy.
			@returns A reference to *this for operator chaining.
			*/
		ETForceInlineHint ::Eldritch2::ErrorCode&	operator=( const ::Eldritch2::ErrorCode& source ) = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

		//!	Gets a human-readable string describing the result condition this @ref ErrorCode instance represents.
		/*!	@returns A null-terminated string of static storage duration containing the error description.
			*/
		const ::Eldritch2::UTF8Char*	ToUTF8String() const;
		
		//!	Returns a POSIX-compliant integer description of the error state.
		ETForceInlineHint int			ToInt() const;

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