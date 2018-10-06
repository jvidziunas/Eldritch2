/*==================================================================*\
  CpuDispatcher.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <task/PxCpuDispatcher.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling {
	using JobFence = Atomic<int>;
}} // namespace Eldritch2::Scheduling

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class CpuDispatcher : public physx::PxCpuDispatcher {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref CpuDispatcher instance.
		CpuDispatcher(physx::PxU32 taskCount) ETNoexceptHint;
		//!	Disable copy construction.
		CpuDispatcher(const CpuDispatcher&) = delete;

		~CpuDispatcher() = default;

		// ---------------------------------------------------

	public:
		Scheduling::JobFence& GetTasksCompletedFence() ETNoexceptHint;

		// - PXCPUDISPATCHER METHODS -------------------------

	public:
		physx::PxU32 getWorkerCount() const override;

		void submitTask(physx::PxBaseTask& task) override;

		// - DATA MEMBERS ------------------------------------

	private:
		Scheduling::JobFence _tasksCompleted;
		physx::PxU32         _taskCount;
	};

}}} // namespace Eldritch2::Physics::PhysX

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/CpuDispatcher.inl>
//------------------------------------------------------------------//
