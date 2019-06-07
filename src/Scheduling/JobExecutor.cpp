/*==================================================================*\
  JobExecutor.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {

	JobExecutor::JobExecutor() ETNoexceptHint : _jobs(MallocAllocator("Worker Context Completion Queue Allocator"), /*capacityHint=*/64u),
												_suspendedJobs(MallocAllocator("Suspended Context Collection Allocator")),
												_pooledFibers(MallocAllocator("Pooled Fiber Collection Allocator")),
												_bootFiber(nullptr),
												_waitFiber(nullptr),
												_switchFiber(nullptr),
												_transitionTarget(nullptr),
												_transitionSource(nullptr, []() -> bool { return false; }) {}

	// ---------------------------------------------------

	void JobExecutor::AwaitCondition_(const char* file, int line, FixedFunction<WaitClosureSizeBytes, bool()> shouldResume) {
		if (shouldResume()) {
			return;
		}

		if (_pooledFibers) {
			_transitionTarget = _pooledFibers.Back();
			_pooledFibers.Pop();
		}

		_transitionSource = SuspendedJob(file, line, GetActiveFiber(), Move(shouldResume));
		ActivateFiber(_waitFiber);
	}

	// ---------------------------------------------------

	void JobExecutor::ResumeOnCaller(PlatformFiber fiber) {
		_transitionTarget = fiber;
		_transitionSource = SuspendedJob(GetActiveFiber(), []() ETNoexceptHint {
			// Fibers suspended via this method are always considered resumable.
			return true;
		});

		ActivateFiber(_switchFiber);
	}

	// ---------------------------------------------------

	void JobExecutor::BootFibers(size_t supportFiberStackSizeInBytes, size_t fiberStackSizeInBytes) {
		enum : SIZE_T { FiberCount = 8u };

		_suspendedJobs.Reserve(FiberCount - 1u);
		_pooledFibers.Reserve(FiberCount);

		_waitFiber = CreatePlatformFiber(supportFiberStackSizeInBytes, this, [](void* self) ETNoexceptHint {
			ETInfiniteLoop {
				// Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.
				static_cast<JobExecutor*>(self)->_suspendedJobs.Append(static_cast<JobExecutor*>(self)->_transitionSource);
				ActivateFiber(static_cast<JobExecutor*>(self)->_transitionTarget);
			}
		});

		_switchFiber = CreatePlatformFiber(supportFiberStackSizeInBytes, this, [](void* self) ETNoexceptHint {
			ETInfiniteLoop {
				// Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.
				static_cast<JobExecutor*>(self)->_pooledFibers.Append(static_cast<JobExecutor*>(self)->_transitionSource.fiber);
				ActivateFiber(static_cast<JobExecutor*>(self)->_transitionTarget);
			}
		});

		for (size_t index(0u); index < FiberCount; ++index) {
			_pooledFibers.Append(CreatePlatformFiber(fiberStackSizeInBytes, this, [](void* self) ETNoexceptHint {
				static_cast<JobExecutor*>(self)->FiberEntryPoint();
			}));
		}
	}

}} // namespace Eldritch2::Scheduling
