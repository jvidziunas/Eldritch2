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
#include <Common/Atomic.hpp>
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

	Atomic<AssertionHandler> handler = [](StringView condition, StringView file, uint32 line, StringView message) -> AssertionFailure {
#if ET_PLATFORM_WINDOWS
		fmt::memory_buffer string;
		fmt::format_to(string, fmt::string_view("{}({}): [{}] {}"), file, line, condition, message);
		OutputDebugStringA(string.data());
#endif

		return AssertionFailure::Fatal;
	};

} // anonymous namespace

AssertionHandler InstallHandler(AssertionHandler newHandler) {
	return handler.exchange(newHandler, std::memory_order_acq_rel);
}

// ---------------------------------------------------

AssertionFailure ReportFailure(StringView condition, StringView file, uint32 line, StringView message) {
	return handler.load(std::memory_order_acquire)(condition, file, line, message);
}

} // namespace Eldritch2
