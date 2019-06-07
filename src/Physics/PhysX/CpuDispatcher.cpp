/*==================================================================*\
  CpuDispatcher.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/CpuDispatcher.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Core/Profiler.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <task/PxTask.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::physx;

	CpuDispatcher::CpuDispatcher() ETNoexceptHint : _tasksCompleted(0) {}

	// ---------------------------------------------------

	PxU32 CpuDispatcher::getWorkerCount() const ETNoexceptHint {
		return Maximum(GetCoreInfo().physicalCores - 1u, 1u);
	}

	// ---------------------------------------------------

	void CpuDispatcher::submitTask(PxBaseTask& task) ETNoexceptHint {
		GetExecutor()->StartAsync(_tasksCompleted, [&](JobExecutor& /*executor*/) ETNoexceptHint {
			ET_PROFILE_SCOPE("PhysX", "PhysX Task (Opaque)", 0x76b900);

			task.run();
			task.release();
		});
	}

}}} // namespace Eldritch2::Physics::PhysX
