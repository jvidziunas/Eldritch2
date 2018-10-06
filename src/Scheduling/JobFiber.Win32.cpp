/*==================================================================*\
  JobExecutor.Win32.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {

	namespace {

		static const DWORD ActiveFiberFlsIndex(FlsAlloc(nullptr));

	} // anonymous namespace

	void JobExecutor::AwaitCondition_(const char* file, int line, Function<bool() ETNoexceptHint> shouldResume) {
		if (shouldResume()) {
			return;
		}

		if (_pooledFibers) {
			_transitionTarget = _pooledFibers.Front();
			_pooledFibers.EraseUnordered(_pooledFibers.Begin());
		}

		_transitionSource = SuspendedJob(file, line, GetCurrentFiber(), eastl::move(shouldResume));
		SwitchToFiber(_waitFiber);
	}

	// ---------------------------------------------------

	void JobExecutor::SwitchFibers(Detail::PlatformFiber targetFiber) {
		_transitionTarget = targetFiber;
		_transitionSource = SuspendedJob {
			/*file =*/"",
			/*line =*/-1,
			/*fiber =*/GetCurrentFiber(),
			/*shouldResume =*/[]() { return true; } // Fibers suspended via this method are considered always resumable.
		};

		SwitchToFiber(_switchFiber);
	}

	// ---------------------------------------------------

	void JobExecutor::BootFibers(size_t supportFiberStackSizeInBytes, size_t fiberStackSizeInBytes) {
		enum : SIZE_T { FiberCount = 8u };

		const auto JobFiber([](void* executor) {
			auto self(static_cast<JobExecutor*>(executor));
			ETInfiniteLoop {
				//	Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.
				self->_suspendedJobs.Append(self->_transitionSource);
				SwitchToFiber(self->_transitionTarget);
			}
		});
		const auto SwitchFiber([](void* executor) {
			auto self(static_cast<JobExecutor*>(executor));
			ETInfiniteLoop {
				//	Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.
				self->_pooledFibers.Append(self->_transitionSource.fiber);
				SwitchToFiber(self->_transitionTarget);
			}
		});

		_suspendedJobs.Reserve(FiberCount - 1u);
		_pooledFibers.Reserve(FiberCount);

		_waitFiber   = CreateFiberEx(0u, supportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, JobFiber, this);
		_switchFiber = CreateFiberEx(0u, supportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, SwitchFiber, this);
		for (size_t index(0u); index < FiberCount; ++index) {
			_pooledFibers.Append(CreateFiberEx(0u, fiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [](void* fiber) { static_cast<JobExecutor*>(fiber)->FiberEntryPoint(); }, this));
		}
	}

	// ---------------------------------------------------

	void JobExecutor::SetActive() {
		FlsSetValue(ActiveFiberFlsIndex, this);
	}

	// ---------------------------------------------------

	JobExecutor* GetExecutor() ETNoexceptHint {
		return static_cast<JobExecutor*>(FlsGetValue(ActiveFiberFlsIndex));
	}

}} // namespace Eldritch2::Scheduling
