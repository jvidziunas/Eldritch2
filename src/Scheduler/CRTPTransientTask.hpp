/*==================================================================*\
  CRTPTransientTask.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/Task.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduler {

	template <typename ImplementingTask>
	class CRTPTransientTask : public Scheduler::Task {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref CRTPTransientTask instance.
		/*!	@param[in] dependentTask @ref Task to (potentially) be started once the new @ref CRTPTransientTask finishes execution.
			@param[in] codependentTask @ref Task that cannot finish until the new @ref CRTPTransientTask has also completed.
			@remarks Codependency and dependency are mutually-exclusive states for task graphs. The parameters will be checked for this in debug mode.
			@remarks Designed to be called within subclasses only.
			*/
		ETForceInlineHint CRTPTransientTask( Scheduler::Task& dependentTask, Scheduler::Task& codependentTask );
		//!	Constructs this @ref CRTPTransientTask instance.
		/*!	@param[in] codependentTask @ref Task that cannot finish until the new @ref CRTPTransientTask has also completed.
			@remarks Designed to be called within subclasses only.
			*/
		ETForceInlineHint CRTPTransientTask( Scheduler::Task& codependentTask, const CodependentTaskSemantics );
		//! Constructs this @ref CRTPTransientTask instance.
		/*!	@param[in] dependentTask @ref Task to (potentially) be started once the new @ref CRTPTransientTask finishes execution.
			@remarks Designed to be called within subclasses only.
			*/
		ETForceInlineHint CRTPTransientTask( Scheduler::Task& dependentTask, const ContinuationTaskSemantics );
		//! Constructs this @ref CRTPTransientTask instance.
		/*!	@remarks Designed to be called within subclasses only.
			*/
		ETForceInlineHint CRTPTransientTask();

		//! Destroys this @ref CRTPTransientTask instance.
		/*!	@remarks Designed to be called within subclasses only.
			*/
		~CRTPTransientTask() = default;

	// ---------------------------------------------------

	public:
		void	Finalize( Scheduler::WorkerContext& executingContext ) override;
	};

}	// namespace Scheduler
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduler/CRTPTransientTask.inl>
//------------------------------------------------------------------//