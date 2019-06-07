/*==================================================================*\
  FiberJobSystem.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides a scheduler implementation policy that wraps Windows
  specific implementations of job queuing functionalities.

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Scheduling/JobSystem.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling { namespace Win32 {

	class FiberJobSystem : public Scheduling::JobSystem {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class JobThread : public Thread, public Scheduling::JobExecutor {
			// - TYPE PUBLISHING ---------------------------------

		public:
			enum class TransferState : uint32 {
				AwaitingTransfer,
				Complete,
				Available
			};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref JobThread instance.
			JobThread(FiberJobSystem& owner) ETNoexceptHint;
			//!	Constructs this @ref JobThread instance.
			JobThread(const JobThread&) ETNoexceptHint;

			~JobThread();

			// ---------------------------------------------------

		public:
			void SetShouldShutDown() ETNoexceptHint override sealed;

			// ---------------------------------------------------

		public:
			bool BeginShareWith(JobThread& thief) ETNoexceptHint;

			void StealFrom(JobThread& victim);

			bool ShouldAwaitTransfer() const ETNoexceptHint;

			void EnableSharing() ETNoexceptHint;

			void DisableSharing() ETNoexceptHint;

			// ---------------------------------------------------

		public:
			Result EnterOnCaller() ETNoexceptHint override sealed;

			// ---------------------------------------------------

		protected:
			void FiberEntryPoint() override;

			// - DATA MEMBERS ------------------------------------

		private:
			FiberJobSystem*       _owner;
			Atomic<JobThread*>    _thief;
			size_t                _victimSeed;
			Atomic<RunBehavior>   _runBehavior;
			Atomic<TransferState> _transferCell;
			JobThread*            _victim;
		};

		// ---

	public:
		using ProcessorAffinity = uint64;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FiberJobSystem(const FiberJobSystem&) = delete;
		//!	Constructs this @ref FiberJobSystem instance.
		FiberJobSystem() ETNoexceptHint;

		~FiberJobSystem();

		// ---------------------------------------------------

	public:
		template <typename WorkItem>
		Result BootOnCaller(ArrayList<JobThread>::SizeType workerCount, WorkItem bootTask);

		void SetShouldShutDown(Result result) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void BackOff(BackoffCounter& context) ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void SetCallerCoreAffinity(ProcessorAffinity affinity) const;

		void SetCallerExecutionPriority(ThreadExecutionPriority priority) override sealed;

		// ---------------------------------------------------

	public:
		JobThread& FindVictim(size_t& victimSeed) ETNoexceptHint;

		size_t GetMaximumParallelism() const ETNoexceptHint override sealed;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		FiberJobSystem& operator=(const FiberJobSystem&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		JobFence             _dummy;
		ArrayList<JobThread> _workers;
	};

}}} // namespace Eldritch2::Scheduling::Win32

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.inl>
//------------------------------------------------------------------//
