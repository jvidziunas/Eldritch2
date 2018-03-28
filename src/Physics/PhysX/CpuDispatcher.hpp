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
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <task/PxCpuDispatcher.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		using JobFence = Atomic<int>;
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class CpuDispatcher : public physx::PxCpuDispatcher {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CpuDispatcher instance.
		CpuDispatcher( physx::PxU32 taskCount );
	//!	Disable copy construction.
		CpuDispatcher( const CpuDispatcher& ) = delete;

		~CpuDispatcher() = default;

	// ---------------------------------------------------

	public:
		ETNeverThrowsHint const Scheduling::JobFence&	GetTasksCompletedFence() const;

	// - PXCPUDISPATCHER METHODS -------------------------

	public:
		physx::PxU32	getWorkerCount() const override;

		void			submitTask( physx::PxBaseTask& task ) override;

	// - DATA MEMBERS ------------------------------------

	private:
		Scheduling::JobFence	_tasksCompleted;
		physx::PxU32			_taskCount;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/CpuDispatcher.inl>
//------------------------------------------------------------------//