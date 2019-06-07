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
	ETInlineHint Result FiberJobSystem::BootOnCaller(ArrayList<JobThread>::SizeType workers, WorkItem bootTask) {
		ETAssert(_workers.IsEmpty(), "Duplicate scheduler boot operation!");

		_workers.Resize(Maximum(workers, 2u), JobThread(*this));
		//	Attach all worker threads that *aren't* run on the main thread...
		for (ArrayList<JobThread>::SizeType worker(1u); worker < _workers.GetSize(); ++worker) {
			_workers[worker].Boot("Worker Thread");
		}

		_workers.Front().StartAsync(_dummy, Move(bootTask));
		return _workers.Front().BootOnCaller("Main Thread");
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint FiberJobSystem::JobThread& FiberJobSystem::FindVictim(size_t& seed) ETNoexceptHint {
		static ETConstexpr auto GetRandom([](size_t& seed) ETNoexceptHint -> size_t {
			enum : size_t {
				Shift = size_t(sizeof(size_t) == sizeof(uint64) ? 32u : 16u),
				Mask  = size_t(sizeof(size_t) == sizeof(uint64) ? 0x7FFFFFFF : 0x7FFF)
			};

			seed = (214013u * seed + 2531011u);
			return (seed >> Shift) & Mask;
		});

		return _workers[GetRandom(seed) % _workers.GetSize()];
	}

}}} // namespace Eldritch2::Scheduling::Win32
