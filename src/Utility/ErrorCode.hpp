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

	enum class Error : Eldritch2::int32 {
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
		ErrorCode( Eldritch2::Error bitfield = Eldritch2::Error::None );
	//!	Constructs this @ref ErrorCode instance.
		ErrorCode( const ErrorCode& ) = default;

		~ErrorCode() = default;

	// - ERROR DETECTION ---------------------------------
		
	public:
	//!	Examines this @ref ErrorCode to determine if it indicates a failure condition.
	/*!	@returns _True_ if this @ref ErrorCode represents success, or _false_ otherwise. */
		explicit operator	bool() const;

	// - ASSIGNMENT --------------------------------------

	public:
		ErrorCode&	operator=( const ErrorCode& ) = default;

	// - DEBUG/LOGGING INFORMATION -----------------------
		
	public:
	//!	Returns a POSIX-compliant integer description of the error state.
		int	AsPosixInt() const;

	//!	Gets a human-readable string describing the result condition this @ref ErrorCode instance represents.
	/*!	@returns A null-terminated string of static storage duration containing the error description. */
		friend ETPureFunctionHint const Eldritch2::Utf8Char*	GetErrorString( const ErrorCode& );

		friend ETPureFunctionHint bool							operator==( const ErrorCode&, const ErrorCode& );

	// - DATA MEMBERS ------------------------------------
		
	private:
		Eldritch2::int32	_value;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/ErrorCode.inl>
//------------------------------------------------------------------//