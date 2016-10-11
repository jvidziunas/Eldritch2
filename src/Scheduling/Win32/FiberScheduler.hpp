/*==================================================================*\
  FiberScheduler.hpp
  ------------------------------------------------------------------
  Purpose:
  Provides a scheduler implementation policy that wraps Windows
  specific implementations of job queuing functionalities.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/ThreadScheduler.hpp>
#include <Utility/UniquePointer.hpp>
#include <Scheduling/JobFiber.hpp>
#include <Scheduling/Thread.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {

	class FiberScheduler : public Scheduling::ThreadScheduler {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ProcessorAffinity	= Eldritch2::uint64;
		using PlatformFiber		= void*;

	// ---

		class WorkerThread : public Scheduling::Thread, public Scheduling::JobFiber {
		// - TYPE PUBLISHING ---------------------------------

		public:
			struct SuspendedFiber {
			// - TYPE PUBLISHING ---------------------------------

			public:
				using TestResumableFunction = bool (*)( const SuspendedFiber& );
				
			// - DATA MEMBERS ------------------------------------

				PlatformFiber			fiber;
				JobBarrier*				barrier;
				int						desiredValue;

				TestResumableFunction	testResumableFunction;
			};

		// ---

			enum class TaskTransferStatus : Eldritch2::uint32 {
				AwaitingTransfer,
				Complete,
				Available
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref WorkerThread instance.
			WorkerThread( FiberScheduler& scheduler );
		//!	Disable copying.
			WorkerThread( const WorkerThread& ) = delete;

			~WorkerThread();

		// ---------------------------------------------------

		public:
			Eldritch2::Utf8Literal	GetHumanReadableName() const override sealed;

		// ---------------------------------------------------

		public:
			void	Run() override sealed;

			void	RequestGracefulShutdown() override sealed;

		// ---------------------------------------------------

		public:
			static WorkerThread*	GetActiveWorkerThread();

		// ---------------------------------------------------

		public:
			void	AwaitBarrier( JobBarrier& barrier, int value = 0 ) override sealed;

		// ---------------------------------------------------

		public:
			bool	TryBeginSharing( WorkerThread& thief );

			void	StealTask( FiberScheduler& scheduler );

			void	CompleteStealOperation( WorkerThread& thief );

		// ---------------------------------------------------

		public:
			void	SwitchFibers( PlatformFiber targetFiber );

		protected:
			void	FiberEntryPoint();

		// - DATA MEMBERS ------------------------------------

		private:
			Eldritch2::ResizableArray<SuspendedFiber>	_suspendedFibers;
			Eldritch2::ResizableArray<PlatformFiber>	_pooledFibers;

			std::atomic<WorkerThread*>					_thief;
			size_t										_randomWorkerSeed;
			std::atomic<TerminationBehavior>			_terminationBehavior;
			std::atomic<TaskTransferStatus>				_transferCell;

			PlatformFiber								_bootFiber;
			PlatformFiber								_waitFiber;
			PlatformFiber								_switchFiber;

			PlatformFiber								_targetFiber;
			
			SuspendedFiber								_sourceFiber;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref FiberScheduler instance.
		FiberScheduler( Eldritch2::Allocator& allocator );
	//!	Disable copying.
		FiberScheduler( const FiberScheduler& ) = delete;
		
	//!	Destroys this @ref FiberScheduler instance.
		~FiberScheduler();

	// - THREAD SCHEDULING -------------------------------

	public:
		Eldritch2::ErrorCode	EnqueueOnCaller( Scheduling::Thread& thread ) override sealed;

		Eldritch2::ErrorCode	Enqueue( Scheduling::Thread& thread ) override sealed;

	// ---------------------------------------------------

	public:
		template <typename WorkItem>
		void	Bootstrap( size_t workerCount, WorkItem&& initialTask );

		void	ShutDown() override sealed;

	// ---------------------------------------------------

	public:
		void	Backoff( BackoffContext& context ) override sealed;

	// ---------------------------------------------------

	public:
		void	SetCallerCoreAffinity( ProcessorAffinity affinity ) const;

		void	SetCallerExecutionPriority( ThreadExecutionPriority priority ) override sealed;

	// ---------------------------------------------------

	public:
		WorkerThread&	GetRandomWorker( const WorkerThread& executingWorker, size_t& randomSeed );

		size_t			GetMaximumParallelism() const override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator			_allocator;
	/*!	Collection of worker threads this scheduler instance uses to execute job closures.
	 *	DO NOT MODIFY OUTSIDE OF @ref Bootstrap(). */
		Eldritch2::UniquePointer<WorkerThread[]>	_workers;
		Scheduling::JobBarrier						_dummyBarrier;
	};

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/Win32/FiberScheduler.inl>
//------------------------------------------------------------------//