/*==================================================================*\
  Thread.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Profiler.hpp>
#include <Common/Thread.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETThreadLocal Thread* CurrentThread(nullptr);

ErrorCode Thread::BootOnCaller(AbstractStringView<Utf8Char> name) {
	CurrentThread = this;
	NameThread(name);
#if ET_DEBUG_BUILD
	const ExecutionState previous(_state.exchange(ExecutionState::Running, std::memory_order_acq_rel));
	ET_ASSERT(previous == ExecutionState::Uninitialized, "Duplicate thread start operation!");
#else
	_state.store(ExecutionState::Running, std::memory_order_release);
#endif
	ET_VERIFY(SUCCEEDED(CoInitializeEx(/*pvReserved =*/nullptr, /*dwCoInit =*/0)), "Error in CoInitializeEx()!");
	EnterOnCaller();
	CoUninitialize();
	ET_PROFILE_THREAD_END();
	_state.store(ExecutionState::Terminated, std::memory_order_release);
	CurrentThread = nullptr;

	return Error::None;
}

// ---------------------------------------------------

ErrorCode Thread::Boot(AbstractStringView<Utf8Char> name) {
	struct CreateInfo {
		AbstractStringView<Utf8Char> name;
		Thread*                      self;
		Atomic<bool>                 started;
	} createInfo = {
		/*scheduler =*/name,
		/*thread =*/this,
		/*started =*/false
	};

	const auto entryPoint([](void* createInfo) -> unsigned int {
		register Thread* const self(static_cast<CreateInfo*>(createInfo)->self);
		CurrentThread = self;
		NameThread(static_cast<CreateInfo*>(createInfo)->name);
#if ET_DEBUG_BUILD
		const ExecutionState previous(self->_state.exchange(ExecutionState::Running, std::memory_order_acq_rel));
		ET_ASSERT(previous == ExecutionState::Uninitialized, "Duplicate thread start operation!");
#else
		self->_state.store(ExecutionState::Running, std::memory_order_release);
#endif
		ET_VERIFY(SUCCEEDED(CoInitializeEx(/*pvReserved =*/nullptr, /*dwCoInit =*/0)), "Error in CoInitializeEx()!");
		static_cast<CreateInfo*>(createInfo)->started.store(true, std::memory_order_release);
		self->EnterOnCaller();
		CoUninitialize();
		ET_PROFILE_THREAD_END();
		self->_state.store(ExecutionState::Terminated, std::memory_order_release);
		CurrentThread = nullptr;

		return 0;
	});

	if (_beginthreadex(nullptr, 0, entryPoint, &createInfo, 0, nullptr) == 0u) {
		switch (errno) {
		case EINVAL: return Error::InvalidParameter;
		case EACCES: return Error::OutOfMemory;
		default: return Error::Unspecified;
		} // switch( errno )
	}

	while (!createInfo.started.load(std::memory_order_consume)) {
		//	Wait until the thread signals it's copied the information safely.
		SwitchToThread();
	}

	return Error::None;
}

// ---------------------------------------------------

ETNoInlineHint void Thread::NameThread(AbstractStringView<Utf8Char> threadName) ETNoexceptHint {
	enum { MS_VC_EXCEPTION = 0x406D1388 };

	char* const name(ETStackAlloc(char, threadName.GetLength() + 1));
	threadName.Copy(name, threadName.GetLength());
	name[threadName.GetLength()] = '\0';

#pragma pack(push, 8)
	//	Taken from http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
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
	/*	(6312, 6322) MSVC warns about the SEH code used here to name a thread in the debugger. This is safe and is taken from official
	 *	documentation, so disable the warnings here. */
	ET_PUSH_MSVC_WARNING_STATE(disable : 6312 6322)
	if (IsDebuggerPresent() != FALSE) {
		__try {
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), PULONG_PTR(&info));
		} __except (EXCEPTION_CONTINUE_EXECUTION) {
		}
	}
	ET_POP_MSVC_WARNING_STATE()

	ET_PROFILE_THREAD_BEGIN(name);
}

// ---------------------------------------------------

Thread* GetCurrentThread() ETNoexceptHint {
	return CurrentThread;
}

} // namespace Eldritch2
