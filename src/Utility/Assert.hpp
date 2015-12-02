/*==================================================================*\
  Assert.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides faculties for testing various conditions in standard
  C/C++ code at both compile and runtime, aborting execution if the
  specified condition(s) are not met and emitting an error message.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <cstdlib>
//------------------------------------------------------------------//

namespace Eldritch2 {

	enum AssertionFailure : ::Eldritch2::uint8 {
		FATAL		= 0,
		NONFATAL	= 1
	};

// ---

	using AssertionHandler	= AssertionFailure (*)( const char*, const char*, uint32, const char* );

// ---------------------------------------------------

	AssertionHandler	GetAssertHandler();

	void				InstallHandler( AssertionHandler newHandler );

// ---------------------------------------------------

	AssertionFailure	ReportFailure( const char* condition, const char* file, ::Eldritch2::uint32 line, const char* message, ... );

#if( ET_DEBUG_MODE_ENABLED || defined( ET_FORCE_ASSERTIONS ) )
#	define ETRuntimeAssert(cond)	ET_MULTILINE_MACRO_BEGIN \
									if( !(cond) ) { \
										if( !Eldritch2::ReportFailure( #cond, __FILE__, static_cast<::Eldritch2::uint32>(__LINE__), nullptr ) ) \
											ET_TRIGGER_DEBUGBREAK(); \
									} \
									ET_MULTILINE_MACRO_END
#else
#	if defined( ET_COMPILER_IS_MSVC )
#		define ETRuntimeAssert(cond)	static_cast<void>(sizeof((cond)));
#	else
#		define ETRuntimeAssert(cond)	ET_MULTILINE_MACRO_BEGIN \
										static_cast<void>(sizeof((cond))); \
										ET_MULTILINE_MACRO_END
#	endif
#endif

#define ETRuntimeVerification(cond)	ET_MULTILINE_MACRO_BEGIN \
									if( !(cond) ) { \
										if( !Eldritch2::ReportFailure( #cond, __FILE__, static_cast<::Eldritch2::uint32>(__LINE__), nullptr ) ) \
											ET_DEBUG_MODE_ENABLED ? ET_TRIGGER_DEBUGBREAK() : ::std::exit(1); \
									} \
									ET_MULTILINE_MACRO_END

#define ETRuntimeVerificationWithMsg(cond, msg)	ET_MULTILINE_MACRO_BEGIN \
												if( !(cond) ) { \
													if( !Eldritch2::ReportFailure( #cond, __FILE__, static_cast<::Eldritch2::uint32>(__LINE__), msg ) ) \
														ET_DEBUG_MODE_ENABLED ? ET_TRIGGER_DEBUGBREAK() : ::std::exit(1); \
												} \
												ET_MULTILINE_MACRO_END

}	// namespace Eldritch2