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

	namespace Detail {

#if ET_ENABLE_JOB_DEBUGGING
		uint64 GetWaitSerialNumber() {
			static ETThreadLocal uint64 counter = 0u;

			return counter++;
		}
#endif

	} // namespace Detail

	JobExecutor::JobExecutor() :
		_jobs(MallocAllocator("Worker Context Completion Queue Allocator")),
		_suspendedJobs(MallocAllocator("Suspended Context Collection Allocator")),
		_pooledFibers(MallocAllocator("Pooled Fiber Collection Allocator")),
		_bootFiber(nullptr),
		_waitFiber(nullptr),
		_switchFiber(nullptr),
		_transitionTarget(nullptr) {
		_jobs.Reserve(64u);
	}

}} // namespace Eldritch2::Scheduling
