/*==================================================================*\
  FiberJobSystem.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling { namespace Win32 {

	namespace {

		ETConstexpr ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetRandom(size_t& seed) {
			enum : size_t {
				Shift = size_t(sizeof(size_t) == sizeof(uint64) ? 32u : 16u),
				Mask  = size_t(sizeof(size_t) == sizeof(uint64) ? 0x7FFFFFFF : 0x7FFF)
			};

			seed = (214013u * seed + 2531011u);
			return (seed >> Shift) & Mask;
		}

	} // anonymous namespace

	ETInlineHint ETForceInlineHint bool FiberJobSystem::JobThread::ShouldAwaitTransfer() const ETNoexceptHint {
		return _transferCell.load(std::memory_order_consume) == TransferState::AwaitingTransfer;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void FiberJobSystem::JobThread::EnableSharing() ETNoexceptHint {
		_thief.store(nullptr, std::memory_order_release);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void FiberJobSystem::JobThread::DisableSharing() ETNoexceptHint {
		if (JobThread* const thief = _thief.exchange(this, std::memory_order_consume)) {
			thief->_transferCell.store(TransferState::Complete, std::memory_order_relaxed);
		}
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void FiberJobSystem::JobThread::SetShouldShutDown() ETNoexceptHint {
		_runBehavior.store(RunBehavior::Terminate, std::memory_order_release);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool FiberJobSystem::JobThread::BeginShareWith(JobThread& thief) ETNoexceptHint {
		JobThread* expected(nullptr);
		return _thief.compare_exchange_weak(expected, ETAddressOf(thief), std::memory_order_acq_rel, std::memory_order_relaxed);
	}

	// ---------------------------------------------------

	template <typename WorkItem>
	ETInlineHint ErrorCode FiberJobSystem::BootOnCaller(size_t totalWorkerCount, WorkItem initialTask) {
		ET_ASSERT(_workers.IsEmpty(), "Duplicate scheduler boot operation!");

		_workers.Resize(Max<size_t>(totalWorkerCount, 2u), JobThread(*this));
		//	Attach all worker threads that *aren't* run on the main thread...
		for (size_t worker(1u); worker < _workers.GetSize(); ++worker) {
			_workers[worker].Boot("Worker Thread");
		}
		//	Queue the first task...
		_workers.Front().StartAsync(_dummy, eastl::move(initialTask));
		return _workers.Front().BootOnCaller("Main Thread");
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint FiberJobSystem::JobThread& FiberJobSystem::FindVictim(size_t& victimSeed) ETNoexceptHint {
		return _workers[GetRandom(victimSeed) % _workers.GetSize()];
	}

}}} // namespace Eldritch2::Scheduling::Win32
