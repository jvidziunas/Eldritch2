/*==================================================================*\
  FiberJobSystem.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
#include <Objbase.h>
#include <Process.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling { namespace Win32 {

	FiberJobSystem::FiberJobSystem() ETNoexceptHint : _workers(MallocAllocator("Job System Worker Thread List Allocator")) {}

	// ---------------------------------------------------

	FiberJobSystem::~FiberJobSystem() {
		for (JobThread& thread : _workers) {
			thread.AwaitCompletion();
		}
	}

	// ---------------------------------------------------

	void FiberJobSystem::SetShouldShutDown(Result /*result*/) ETNoexceptHint {
		for (JobThread& thread : _workers) {
			thread.SetShouldShutDown();
		}
	}

	// ---------------------------------------------------

	void FiberJobSystem::BackOff(BackoffCounter& counter) ETNoexceptHint {
		const BackoffCounter backoff(counter++);

		if (backoff < 10u) {
			_mm_pause();
		} else if (backoff < 20u) {
			for (int i(0u); i != 25u; ++i) {
				_mm_pause();
				_mm_pause();
			}
		} else if (backoff < 22u) {
			SwitchToThread();
		} else if (backoff < 24u) {
			Sleep(0u);
		} else if (backoff < 26u) {
			Sleep(1u);
		} else {
			Sleep(10u);
		}
	}

	// ---------------------------------------------------

	void FiberJobSystem::SetCallerCoreAffinity(ProcessorAffinity affinity) const {
		SetThreadIdealProcessor(GetCurrentThread(), DWORD(affinity));
	}

	// ---------------------------------------------------

	void FiberJobSystem::SetCallerExecutionPriority(ThreadExecutionPriority priority) {
		static ETConstexpr int priorities[size_t(ThreadExecutionPriority::COUNT)] = {
			THREAD_PRIORITY_BELOW_NORMAL,
			THREAD_PRIORITY_NORMAL,
			THREAD_PRIORITY_ABOVE_NORMAL,
			THREAD_PRIORITY_TIME_CRITICAL
		};

		SetThreadPriority(GetCurrentThread(), priorities[size_t(priority)]);
	}

	// ---------------------------------------------------

	size_t FiberJobSystem::GetMaximumParallelism() const ETNoexceptHint {
		return _workers.GetSize();
	}

}}} // namespace Eldritch2::Scheduling::Win32
