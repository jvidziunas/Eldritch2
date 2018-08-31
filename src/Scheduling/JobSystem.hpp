/*==================================================================*\
  JobSystem.hpp
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

namespace Eldritch2 { namespace Scheduling {

	enum class ThreadExecutionPriority {
		BelowNormal,
		Normal,
		AboveNormal,
		Priority,

		COUNT
	};

	// ---

	using BackoffContext = uintptr;

	// ---------------------------------------------------

	class ETPureAbstractHint JobSystem {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref JobSystem instance.
		JobSystem(const JobSystem&) = default;
		//!	Constructs this @ref JobSystem instance.
		JobSystem() = default;

		~JobSystem() = default;

		// ---------------------------------------------------

	public:
		virtual void SetShouldShutDown(ErrorCode result) ETNoexceptHint abstract;

		virtual void BackOff(BackoffContext& context) ETNoexceptHint;

		virtual void SetCallerExecutionPriority(ThreadExecutionPriority priority);

		// ---------------------------------------------------

	public:
		virtual size_t GetMaximumParallelism() const ETNoexceptHint abstract;
	};

}} // namespace Eldritch2::Scheduling
