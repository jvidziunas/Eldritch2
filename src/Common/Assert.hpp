/*==================================================================*\
  Assert.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides faculties for testing various conditions in standard
  C/C++ code at both compile and runtime, aborting execution if the
  specified condition(s) are not met and emitting an error message.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/String.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

#if ET_DEBUG_BUILD || defined(ET_FORCE_ASSERTIONS)
#	define ET_ASSERTIONS_ENABLED 1
#else // !ET_DEBUG_BUILD && !defined(ET_FORCE_ASSERTIONS)
#	define ET_ASSERTIONS_ENABLED 0
#endif // ET_DEBUG_BUILD || defined(ET_FORCE_ASSERTIONS)

namespace Eldritch2 {

enum class AssertionFailure : uint8 {
	Fatal    = 0,
	NonFatal = 1
};

// ---

using AssertionHandler = AssertionFailure (*)(StringSpan /*message*/);

// ---------------------------------------------------

template <typename... Arguments>
void ReportFailure(StringSpan file, uint32 line, StringSpan format, Arguments&&... arguments);

// ---------------------------------------------------

AssertionHandler InstallHandler(AssertionHandler newHandler) ETNoexceptHint;

} // namespace Eldritch2

#if ET_ASSERTIONS_ENABLED
#	define ETAssert(cond, message, ...) if (!(cond)) { ::Eldritch2::ReportFailure(__FILE__, ::Eldritch2::uint32(__LINE__), message, __VA_ARGS__); }
#else // !ET_ASSERTIONS_ENABLED
#	define ETAssert(cond, message, ...) static_cast<void>(sizeof((cond))); __assume(!(cond));
#endif // ET_ASSERTIONS_ENABLED

#define ETVerify(cond, message, ...) if (!(cond)) { ::Eldritch2::ReportFailure(__FILE__, ::Eldritch2::uint32(__LINE__), message, __VA_ARGS__); }

#define ETStaticAssert(cond, message) static_assert(cond, message)

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Assert.inl>
//------------------------------------------------------------------//
