/*==================================================================*\
  JobExecutor.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {

	JobExecutor::JobExecutor() :
		_jobs(MallocAllocator("Worker Context Completion Queue Allocator"), /*capacityHint=*/64u),
		_suspendedJobs(MallocAllocator("Suspended Context Collection Allocator")),
		_pooledFibers(MallocAllocator("Pooled Fiber Collection Allocator")),
		_bootFiber(nullptr),
		_waitFiber(nullptr),
		_switchFiber(nullptr),
		_transitionTarget(nullptr) {}

}} // namespace Eldritch2::Scheduling
