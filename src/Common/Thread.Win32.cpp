/*==================================================================*\
  Thread.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Windows.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//

#define ET_PROFILE_THREAD_END()
#define ET_PROFILE_THREAD_BEGIN(name)

namespace Eldritch2 {

static ETThreadLocal Thread* CurrentThread(nullptr);

Result Thread::BootOnCaller(AbstractStringSpan<Utf8Char> name) ETNoexceptHint {
	NameThread(name);
	ET_PROFILE_THREAD_BEGIN(name);
	ET_AT_SCOPE_EXIT(ET_PROFILE_THREAD_END());

	CurrentThread = this;
	_state.store(ExecutionState::Running, std::memory_order_release);
	ET_AT_SCOPE_EXIT(Exchange(CurrentThread, nullptr)->_state.store(ExecutionState::Terminated, std::memory_order_release));
	ETVerify(SUCCEEDED(CoInitializeEx(/*pvReserved =*/nullptr, /*dwCoInit =*/0)), "Error in CoInitializeEx()!");
	ET_AT_SCOPE_EXIT(CoUninitialize());

	return EnterOnCaller();
}

// ---------------------------------------------------

Result Thread::Boot(AbstractStringSpan<Utf8Char> name) ETNoexceptHint {
	using CreateInfo = Tuple<Thread*, AbstractStringSpan<Utf8Char>>;

	const auto EntryPoint([](void* createInfo) ETNoexceptHint -> unsigned int {
		const CreateInfo& info(*static_cast<const CreateInfo*>(createInfo));
		return (unsigned int)(Get<Thread*>(info)->BootOnCaller(Get<AbstractStringSpan<Utf8Char>>(info)));
	});

	// ---

	ETAssert(GetState() == ExecutionState::Uninitialized, "Duplicate thread start operation for thread {}", fmt::ptr(this));

	CreateInfo createInfo(MakeTuple(this, name));
	if (_beginthreadex(nullptr, /*stackSize =*/0, EntryPoint, ETAddressOf(createInfo), /*flags =*/0, /*thread =*/nullptr) == 0u) {
		switch (errno) {
		case EINVAL: return Result::InvalidParameter;
		case EACCES: return Result::OutOfMemory;
		default: return Result::Unspecified;
		} // switch( errno )
	}

	//	Wait until the thread signals it's copied the information safely.
	while (GetState() == ExecutionState::Uninitialized) {
		SwitchToThread();
	}

	return Result::Success;
}

// ---------------------------------------------------

ETNoInlineHint void Thread::NameThread(AbstractStringSpan<Utf8Char> threadName) ETNoexceptHint {
	char* const name(ETStackAlloc(char, threadName.GetLength() + 1));
	name[threadName.Copy(name, threadName.GetLength())] = '\0';

	//	Taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
#pragma pack(push, 8)
	struct {
		DWORD  dwType;     // Must be 0x1000.
		LPCSTR szName;     // Pointer to name (in user address space).
		DWORD  dwThreadID; // Thread ID (-1 = caller thread).
		DWORD  dwFlags;    // Reserved for future use, must be 0.
	} info = {
		0x1000,
		name,
		DWORD(-1),
		0u
	};
#pragma pack(pop)
	/*	(6312, 6322) MSVC warns about the SEH handler used to name a thread in the debugger. This is safe and is taken from official
	 *	documentation, so disable the warnings here. */
	ET_PUSH_MSVC_WARNING_STATE(disable : 6312 6322)
	if (IsDebuggerPresent() != FALSE) {
		__try {
			RaiseException(/*MS_VC_EXCEPTION*/0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), PULONG_PTR(ETAddressOf(info)));
		} __except (EXCEPTION_CONTINUE_EXECUTION) {
		}
	}
	ET_POP_MSVC_WARNING_STATE()
}

} // namespace Eldritch2
