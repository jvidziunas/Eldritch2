/*==================================================================*\
  CpuDispatcher.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/CpuDispatcher.hpp>
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
		We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE(disable : 6326)
#	include <task/PxTask.h>
	ET_POP_COMPILER_WARNING_STATE()
	//------------------------------------------------------------------//

	using namespace ::Eldritch2::Scheduling;
using namespace ::physx;

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {

			CpuDispatcher::CpuDispatcher(PxU32 taskCount) : _taskCount(taskCount), _tasksCompleted(0) {}

		// ---------------------------------------------------

			PxU32 CpuDispatcher::getWorkerCount() const {
				return _taskCount;
			}

		// ---------------------------------------------------

			void CpuDispatcher::submitTask(PxBaseTask& task) {
				GetExecutor().StartAsync(_tasksCompleted, [&task](JobExecutor& /*executor*/) {
					MICROPROFILE_SCOPEI("PhysX", task.getName(), 0x76b900);
					task.run();
					task.release();
				});
			}

		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2