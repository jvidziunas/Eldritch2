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
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	enum class Error : int32 {
		Timeout					= -8,
		UnsupportedOperation	= -7,
		AccessDenied			= -6,
		InvalidObjectState		= -5,
		InvalidFileName			= -4,
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
		ErrorCode( Error bitfield = Error::None );
	//!	Constructs this @ref ErrorCode instance.
		ErrorCode( const ErrorCode& ) = default;

		~ErrorCode() = default;

	// - ASSIGNMENT --------------------------------------

	public:
		ErrorCode&	operator=( const ErrorCode& ) = default;

	// - DEBUG/LOGGING INFORMATION -----------------------

	//!	Gets a human-readable string describing the result condition this @ref ErrorCode instance represents.
	/*!	@returns A null-terminated string of static storage duration containing the error description. */
		friend ETPureFunctionHint const Utf8Char*	AsCString( ErrorCode code );

		friend ETPureFunctionHint int				AsPosixInt( ErrorCode code );

		friend ETPureFunctionHint bool				Succeeded( ErrorCode code );

		friend ETPureFunctionHint bool				Failed( ErrorCode code );

		friend ETPureFunctionHint bool				operator==( ErrorCode code, ErrorCode code1 );

	// - DATA MEMBERS ------------------------------------
		
	private:
		int32	_value;
	};

}	// namespace Eldritch2

#if !defined( ET_FAIL_UNLESS )
#	define ET_FAIL_UNLESS(condition) { const auto tempResult( condition ); if(ET_UNLIKELY( Failed( tempResult ) )) { return tempResult; } }
#endif

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/ErrorCode.inl>
//------------------------------------------------------------------//