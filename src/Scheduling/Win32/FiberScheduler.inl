/*==================================================================*\
  FiberScheduler.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Algorithms.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {

	template <typename WorkItem>
	void FiberScheduler::Bootstrap( size_t totalWorkerCount, WorkItem&& initialTask ) {
	//	Make sure we aren't trying to re-bootstrap after the initial launch.
		ETRuntimeAssert( !_workers );

		Scheduling::JobBarrier	dummyBarrier( 0 );

		_workers	= MakeUniqueArray<WorkerThread>( _allocator, Eldritch2::Max<size_t>( totalWorkerCount, 2u ), *this );

	//	Attach all worker threads that *aren't* run on the main thread...
		Eldritch2::ForEach( _workers.Begin() + 1, _workers.End(), [&] ( WorkerThread& worker ) { FiberScheduler::Enqueue( worker ); } );

	//	Queue the first task...
		_workers[0].Enqueue( dummyBarrier, eastl::forward<WorkItem>( initialTask ) );

	// ... and finally kick off the thread!
		FiberScheduler::EnqueueOnCaller( _workers[0] );
	}

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2