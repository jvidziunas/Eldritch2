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

namespace Eldritch2 {
	namespace Scheduling {
		namespace {

			static const DWORD activeFiberFlsIndex = FlsAlloc(nullptr);

		}	// anonymous namespace

		void JobExecutor::Await_(const char* file, int line, Function<bool()> shouldResume) {
			if (shouldResume()) {
				return;
			}

			if (_pooledFibers) {
				_transitionTarget = _pooledFibers.Front();
				_pooledFibers.Erase(_pooledFibers.Begin(), UnorderedSemantics());
			}

			_transitionSource = SuspendedJob(file, line, GetCurrentFiber(), eastl::move(shouldResume));

			SwitchToFiber(_waitFiber);
		}

	// ---------------------------------------------------

		void JobExecutor::SwitchFibers(Detail::PlatformFiber targetFiber) {
			_transitionTarget = targetFiber;
			_transitionSource = SuspendedJob{
				"",
				-1,
				GetCurrentFiber(),
				[]() {
				//	Fibers suspended via this method are considered always resumable.
					return true;
				}
			};

			SwitchToFiber(_switchFiber);
		}

	// ---------------------------------------------------

		void JobExecutor::BootSupportFibers() {
			enum : SIZE_T {
				SupportFiberStackSizeInBytes = 32768u,
				FiberStackSizeInBytes = 524288u,
				FiberCount = 8u
			};

		// ---

			_suspendedJobs.Reserve(FiberCount - 1u);
			_pooledFibers.Reserve(FiberCount);

			_waitFiber = CreateFiberEx(0u, SupportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [](void* fiber) {
				auto& thisFiber(*static_cast<JobExecutor*>(fiber));

			//	Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.
				for (;;) {
				//	Commit the previous fiber into the suspended fiber list.
					thisFiber._suspendedJobs.Append(thisFiber._transitionSource);

					SwitchToFiber(thisFiber._transitionTarget);
				}
			}, this);

			_switchFiber = CreateFiberEx(0u, SupportFiberStackSizeInBytes, FIBER_FLAG_FLOAT_SWITCH, [](void* fiber) {
				auto& thisFiber(*static_cast<JobExecutor*>(fiber));

			//	Fancy loop trick will cause execution to jump back around to here for the next caller after switching away.
				for (;;) {
				//	Commit the previous fiber into the suspended fiber list.
					thisFiber._pooledFibers.Append(thisFiber._transitionSource.fiber);

					SwitchToFiber(thisFiber._transitionTarget);
				}
			}, this);

			for (size_t index(0u); index < FiberCount; ++index) {
				_pooledFibers.Append(
					CreateFiberEx(
					0u,
					FiberStackSizeInBytes,
					FIBER_FLAG_FLOAT_SWITCH,
					[](void* fiber) {
					static_cast<JobExecutor*>(fiber)->FiberEntryPoint();
				},
					this
					)
				);
			}
		}

	// ---------------------------------------------------

		void JobExecutor::SetActive() {
			FlsSetValue(activeFiberFlsIndex, this);
		}

	// ---------------------------------------------------

		JobExecutor& GetExecutor() {
			JobExecutor* const fiber(static_cast<JobExecutor*>(FlsGetValue(activeFiberFlsIndex)));

			ET_ASSERT(fiber != nullptr, "Calling GetExecutor() from a thread not owned by the scheduler!");

			return *fiber;
		}

	}	// namespace Scheduling
}	// namespace Eldritch2