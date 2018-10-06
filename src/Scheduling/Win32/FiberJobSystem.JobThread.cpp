/*==================================================================*\
  FiberJobSystem.JobThread.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling { namespace Win32 {

	FiberJobSystem::JobThread::JobThread(FiberJobSystem& owner) :
		JobExecutor(),
		_owner(ETAddressOf(owner)),
		_thief(this),
		_victimSeed(size_t(__rdtsc())),
		_runBehavior(RunBehavior::Continue),
		_transferCell(TransferState::Complete),
		_victim(nullptr) {
	}

	// ---------------------------------------------------

	FiberJobSystem::JobThread::JobThread(const JobThread& thread) :
		JobThread(*thread._owner) {
	}

	// ---------------------------------------------------

	FiberJobSystem::JobThread::~JobThread() {
		if (_waitFiber) {
			DeleteFiber(_waitFiber);
		}
		if (_switchFiber) {
			DeleteFiber(_switchFiber);
		}
	}

	// ---------------------------------------------------

	void FiberJobSystem::JobThread::StealFrom(JobThread& victim) {
		auto begin(victim._jobs.Begin());
		auto end(victim._jobs.End());

		begin += (end - begin) / 2;

		_jobs.Assign(eastl::make_move_iterator(begin), eastl::make_move_iterator(end));

		//	Ensure all our writes complete before signaling a finished state.
		_transferCell.store(TransferState::Complete, std::memory_order_release);
		victim._jobs.Erase(begin, end);
	}

	// ---------------------------------------------------

	ErrorCode FiberJobSystem::JobThread::EnterOnCaller() {
		BootFibers();

		_bootFiber = ConvertThreadToFiberEx(this, FIBER_FLAG_FLOAT_SWITCH);

		//	Client fibers have relinqished execution; time to shut down.
		while (_pooledFibers) {
			const PlatformFiber fiber(_pooledFibers.Back());
			_pooledFibers.Pop();

			SwitchToFiber(fiber);
			DeleteFiber(_transitionSource.fiber);

			//	We will resume here once one of the work fibers receives the quit signal.
		}

		return ConvertFiberToThread() != FALSE ? Error::None : Error::Unspecified;
	}

	// ---------------------------------------------------

	void FiberJobSystem::JobThread::FiberEntryPoint() {
		SetActive();
		_thief.store(nullptr, std::memory_order_release);

		while (ET_LIKELY(_runBehavior.load(std::memory_order_consume) != RunBehavior::Terminate)) {
			/*	Distribute any work (if it remains) to other worker threads as requested. Prefer the expanded load/store
			 *	over exchange here to avoid false sharing/contention. */
			if (JobThread* thief = _thief.load(std::memory_order_consume)) {
				if (_thief.compare_exchange_weak(thief, nullptr, std::memory_order_acq_rel, std::memory_order_relaxed)) {
					thief->StealFrom(*this);
				}
			}
			/*	Execute one of the following code paths:
			 *	1) Complete an outstanding work item.
			 *	2) Resume a suspended work item.
			 *	3) Steal work. */

			if (_jobs) { // 1)
				/*	Work items may suspend and re-enter this function in a different fiber, so it is important that the element be
				 *	destroyed *before* execution to avoid races. */
				const JobClosure job(eastl::move(_jobs.Back()));
				_jobs.Pop();
				job.work(*this);
				job.completed->fetch_sub(1, std::memory_order_release);

				continue;
			}

			if (_suspendedJobs) { // 2)
				//	Locate a closure capable of resuming by evaluating its condition function.
				/*	Empirically, most stalled work items are jobs waiting for their dependencies to complete in LIFO order. We can exploit this pattern
				 *	to reduce the number of tests by searching the list of suspended fibers in *reverse* order. */
				const auto job(FindIf(_suspendedJobs.ReverseBegin(), _suspendedJobs.ReverseEnd(), [](const SuspendedJob& job) { return job.shouldResume(); }));
				if (job != _suspendedJobs.ReverseEnd()) {
					const Detail::PlatformFiber fiber(job->fiber);
					_suspendedJobs.EraseUnordered(job);

					SwitchFibers(fiber);
				}

				continue;
			}

			{ // 3
				/*	At this point we have no previously queued work items to complete and no suspended tasks are ready to resume. In the interest of
				 *	Pareto optimality, find a victim thread and attempt to load balance work across the two threads by way of a steal-half sharing
				 *	policy. */

				//	Write is not explicitly ordered as transfer cell is only visible to us until we find a victim.
				_transferCell.store(TransferState::AwaitingTransfer, std::memory_order_relaxed);

				DisableSharing();

				JobThread& victim(_owner->FindVictim(_victimSeed));
				if (victim.BeginShareWith(*this)) {
#if ET_ENABLE_JOB_DEBUGGING
					//	Update the victim value for easier inspection in a debugger.
					_victim = &victim;
#endif // ET_ENABLE_JOB_DEBUGGING

					//	TO CONSIDER: Timeout?
					//	Enter idle loop waiting for completion.
					while (ShouldAwaitTransfer() && victim.GetState() == ExecutionState::Running) {
						_mm_pause();
					}

#if ET_ENABLE_JOB_DEBUGGING
					_victim = nullptr;
#endif // ET_ENABLE_JOB_DEBUGGING
				}

				EnableSharing();
			}
		}

		//	Mark ourselves as unable to be stolen from on death-- we're not producing work items any more!
		DisableSharing();

		//	Switch back to the main fiber.
		_transitionSource.fiber = GetCurrentFiber();
		SwitchToFiber(_bootFiber);
	}

}}} // namespace Eldritch2::Scheduling::Win32
