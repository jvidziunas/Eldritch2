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

#if !defined(ET_FORCE_ASSERTIONS)
#	define ET_FORCE_ASSERTIONS 0
#endif

namespace Eldritch2 {

enum AssertionFailure : uint8 {
	Fatal    = 0,
	NonFatal = 1
};

// ---

using AssertionHandler = AssertionFailure (*)(StringView /*condition*/, StringView /*file*/, uint32 /*line*/, StringView /*message*/);

// ---------------------------------------------------

template <typename... Arguments>
AssertionFailure ReportFailure(StringView condition, StringView file, uint32 line, StringView format, Arguments&&... arguments);
AssertionFailure ReportFailure(StringView condition, StringView file, uint32 line, StringView message);

// ---------------------------------------------------

AssertionHandler InstallHandler(AssertionHandler newHandler);

#if ET_DEBUG_BUILD || ET_FORCE_ASSERTIONS
#	define ET_ASSERT(cond, message, ...)                                                                                   \
		if (!(cond) && !::Eldritch2::ReportFailure(#cond, __FILE__, ::Eldritch2::uint32(__LINE__), message, __VA_ARGS__)) { \
			ET_TRIGGER_DEBUGBREAK();                                                                                        \
		}
#else
#	define ET_ASSERT(cond, message, ...)  \
		static_cast<void>(sizeof((cond))); \
		__assume(!(cond));
#endif

#define ET_VERIFY(cond, message, ...)                                                                                   \
	if (!(cond) && !::Eldritch2::ReportFailure(#cond, __FILE__, ::Eldritch2::uint32(__LINE__), message, __VA_ARGS__)) { \
		ETIsDebugBuild() ? ET_TRIGGER_DEBUGBREAK() : std::terminate();                                                  \
	}

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Assert.inl>
//------------------------------------------------------------------//
