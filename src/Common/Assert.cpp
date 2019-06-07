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
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#if ET_PLATFORM_WINDOWS
#	ifndef NOMINMAX
#		define NOMINMAX
#	endif
#	include <Windows.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace {

	Atomic<AssertionHandler> handler = [](StringSpan message) ETNoexceptHint -> AssertionFailure {
#if ET_PLATFORM_WINDOWS
		const int      maxChars(message.GetSize() + 1);
		wchar_t* const displayString(ETStackAlloc(wchar_t, maxChars));

		MultiByteToWideChar(CP_UTF8, 0u, message.GetData(), int(message.GetSize()), displayString, maxChars);
		if (IsDebuggerPresent() || MessageBoxW(nullptr, displayString, nullptr, MB_ICONERROR | MB_RETRYCANCEL | MB_SYSTEMMODAL | MB_SETFOREGROUND) == IDRETRY) {
			OutputDebugStringW(displayString);
			return AssertionFailure::NonFatal;
		}
#endif

		return AssertionFailure::Fatal;
	};

} // anonymous namespace

AssertionHandler InstallHandler(AssertionHandler newHandler) ETNoexceptHint {
	return handler.exchange(newHandler, std::memory_order_acq_rel);
}

// ---------------------------------------------------

void ReportFailure(StringSpan message) ETNoexceptHint {
	if (handler.load(std::memory_order_acquire)(message) == AssertionFailure::NonFatal) {
		ET_TRIGGER_DEBUGBREAK();
	} else {
		std::terminate();
	}
}

} // namespace Eldritch2
