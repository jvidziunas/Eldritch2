/*==================================================================*\
  Assert.cpp
  ------------------------------------------------------------------
  Purpose:
  Provides faculties for testing various conditions in standard
  C/C++ code at both compile and runtime, aborting execution if the
  specified condition(s) are not met and emitting an error message.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Assert.hpp>
#include <Common/Memory.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	static AssertionHandler handler = [](const char* condition, const char* file, uint32 line, const char* message) -> AssertionFailure {
#if ET_PLATFORM_WINDOWS
		fmt::memory_buffer string;

		fmt::format_to(string, fmt::string_view("{}({}): [{}] {}"), file, line, condition, EmptyForNull(message));

		OutputDebugStringA(string.data());
#endif

		return AssertionFailure::Fatal;
	};

} // anonymous namespace

void InstallHandler(AssertionHandler newHandler) {
	handler = newHandler;
}

// ---------------------------------------------------

AssertionFailure ReportFailure(const char* condition, const char* file, uint32 line, const char* message) {
	return handler(condition, file, line, message);
}

} // namespace Eldritch2
