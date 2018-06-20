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
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

#if !defined(ET_FORCE_ASSERTIONS)
#	define ET_FORCE_ASSERTIONS 0
#endif

namespace Eldritch2 {

enum AssertionFailure : uint8 {
	Fatal    = 0,
	NonFatal = 1
};

// ---

using AssertionHandler = AssertionFailure (*)(const char*, const char*, uint32, const char*);

// ---------------------------------------------------

void InstallHandler(AssertionHandler newHandler);

// ---------------------------------------------------

template <size_t formatSize, typename... Arguments>
AssertionFailure ReportFailure(const char* condition, const char* file, uint32 line, const char (&format)[formatSize], Arguments&&... arguments);
AssertionFailure ReportFailure(const char* condition, const char* file, uint32 line, const char* message);

#if ET_DEBUG_BUILD || ET_FORCE_ASSERTIONS
#	define ET_ASSERT(cond, message, ...)                                                                               \
		if (!(cond) && !Eldritch2::ReportFailure(#cond, __FILE__, static_cast<Eldritch2::uint32>(__LINE__), message)) { \
			ET_TRIGGER_DEBUGBREAK();                                                                                    \
		}
#else
#	define ET_ASSERT(cond, message, ...) static_cast<void>(sizeof((cond)))
#endif

#define ET_VERIFY(cond, message, ...)                                                                               \
	if (!(cond) && !Eldritch2::ReportFailure(#cond, __FILE__, static_cast<Eldritch2::uint32>(__LINE__), message)) { \
		ETIsDebugBuild() ? ET_TRIGGER_DEBUGBREAK() : std::terminate();                                              \
	}

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Assert.inl>
//------------------------------------------------------------------//
