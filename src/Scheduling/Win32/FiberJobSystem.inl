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

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {
namespace {

	static ETInlineHint ETPureFunctionHint size_t GetRandom( size_t& seed ) {
		enum : size_t {
			Shift	= static_cast<size_t>(sizeof(size_t) == sizeof(uint64) ? 32u : 16u ),
			Mask	= static_cast<size_t>(sizeof(size_t) == sizeof(uint64) ? 0x7FFFFFFF : 0x7FFF )
		};

		seed = ( 214013u * seed + 2531011u );
		return ( seed >> Shift ) & Mask;
	}

}	// anonymous namespace

	ETInlineHint bool FiberJobSystem::JobThread::ShouldAwaitTransfer() const {
		return TransferState::AwaitingTransfer == _transferCell.load( std::memory_order_consume );
	}
	
// ---------------------------------------------------

	ETInlineHint bool FiberJobSystem::JobThread::BeginShareWith( JobThread& thief ) {
		JobThread*	expected( nullptr );

		return _thief.compare_exchange_weak( expected, &thief, std::memory_order_acq_rel, std::memory_order_relaxed );
	}

// ---------------------------------------------------

	template <typename WorkItem>
	ETInlineHint int FiberJobSystem::Boot( size_t totalWorkerCount, WorkItem&& initialTask ) {
		ET_ASSERT( _workers == nullptr, "Duplicate scheduler boot operation!" );

		JobFence	dummy( 0 );

		_workers = CreateUniqueArray<JobThread>( _allocator, Max<size_t>( totalWorkerCount, 2u ), *this );

	//	Attach all worker threads that *aren't* run on the main thread...
		ForEach( _workers.Begin() + 1, _workers.End(), [this] ( JobThread& worker ) {
			this->Launch( worker );
		} );

	//	Queue the first task...
		_workers[0].StartAsync( dummy, eastl::forward<WorkItem>( initialTask ) );

	// ... and finally kick off the thread!
		this->LaunchOnCaller( _workers[0] );

		return _shutdownCode.load( std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint FiberJobSystem::JobThread& FiberJobSystem::FindVictim( size_t& victimSeed ) {
		return _workers[GetRandom( victimSeed ) % _workers.GetSize()];
	}

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2