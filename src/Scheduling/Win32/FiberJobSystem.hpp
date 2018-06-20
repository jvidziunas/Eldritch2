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
#include <Scheduling/Thread.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scheduling { namespace Win32 {

	class FiberJobSystem : public Scheduling::JobSystem {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ProcessorAffinity = uint64;
		using PlatformFiber     = void*;

		// ---

		class JobThread : public Scheduling::Thread, public Scheduling::JobExecutor {
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
			JobThread(FiberJobSystem& owner);
			//!	Constructs this @ref JobThread instance.
			JobThread(const JobThread&);

			~JobThread();

			// ---------------------------------------------------

		public:
			Utf8Literal GetName() const override sealed;

			// ---------------------------------------------------

		public:
			void Run() override sealed;

			void SetShouldShutDown() override sealed;

			// ---------------------------------------------------

		public:
			bool BeginShareWith(JobThread& thief);

			void StealFrom(JobThread& victim);

			bool ShouldAwaitTransfer() const;

			void EnableSharing();

			void DisableSharing();

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

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FiberJobSystem(const FiberJobSystem&) = delete;
		//!	Constructs this @ref FiberJobSystem instance.
		FiberJobSystem();

		~FiberJobSystem();

		// - THREAD SCHEDULING -------------------------------

	public:
		ErrorCode LaunchOnCaller(Scheduling::Thread& thread) override sealed;

		ErrorCode Launch(Scheduling::Thread& thread) override sealed;

		// ---------------------------------------------------

	public:
		template <typename WorkItem>
		int Boot(size_t workerCount, WorkItem&& initialTask);

		void SetShouldShutDown(int shutdownCode) override sealed;

		// ---------------------------------------------------

	public:
		void BackOff(BackoffContext& context) override sealed;

		// ---------------------------------------------------

	public:
		void SetCallerCoreAffinity(ProcessorAffinity affinity) const;

		void SetCallerExecutionPriority(ThreadExecutionPriority priority) override sealed;

		// ---------------------------------------------------

	public:
		JobThread& FindVictim(size_t& victimSeed);

		size_t GetMaximumParallelism() const override sealed;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		FiberJobSystem& operator=(const FiberJobSystem&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable MallocAllocator    _allocator;
		UniquePointer<JobThread[]> _workers;
		JobFence                   _dummy;
		Atomic<int>                _shutdownCode;
	};

}}} // namespace Eldritch2::Scheduling::Win32

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.inl>
//------------------------------------------------------------------//
