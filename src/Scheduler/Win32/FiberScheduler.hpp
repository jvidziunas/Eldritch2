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
#include <Scheduler/ThreadScheduler.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Scheduler/Thread.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace System {
		class	SystemInterface;
	}
}

namespace Eldritch2 {
namespace Scheduler {
namespace Win32 {

	class FiberScheduler : public Scheduler::ThreadScheduler {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ProcessorAffinity	= ::Eldritch2::uint64;
		using SystemFiber		= void*;

	// ---

		class WorkerThread : public Scheduler::Thread, public Scheduler::WorkerContext {
		// - TYPE PUBLISHING ---------------------------------

		public:
			struct SuspendedContext {
			// - TYPE PUBLISHING ---------------------------------

				using TestResumableFunction = bool (*)( const SuspendedContext& );
				
			// - DATA MEMBERS ------------------------------------

				SystemFiber				fiber;
				FinishCounter*			counter;
				int						desiredCounterValue;

				TestResumableFunction	testResumableFunction;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref WorkerThread instance.
			WorkerThread( FiberScheduler& scheduler, ::Eldritch2::Allocator& allocator );
			//!	Constructs this @ref WorkerThread instance.
			WorkerThread( const WorkerThread& ) = delete;
			//!	Constructs this @ref WorkerThread instance.
			WorkerThread( WorkerThread&& );

			~WorkerThread();

		// ---------------------------------------------------

			const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

		// ---------------------------------------------------

			void	Run() override sealed;

			void	RequestGracefulShutdown() override sealed;

		// ---------------------------------------------------

			static WorkerThread*	GetActiveWorkerThread();

		// ---------------------------------------------------

			void	WaitForCounter( FinishCounter& finishCounter, int value = 0 ) override sealed;

			void	WaitForCounterLessThan( FinishCounter& finishCounter, int value );

			void	WaitForCounterGreaterThan( FinishCounter& finishCounter, int value );

		// ---------------------------------------------------

			ETInlineHint bool	TryBeginWorkSharingWithThief( WorkerThread& thief );

			void				AcquireReceiverInitiatedStolenTask( FiberScheduler& scheduler );

			void				CompleteWorkSharingRequest( WorkerThread& thief );

		// ---------------------------------------------------

			void	SwitchFibers( const SystemFiber targetFiber );

		protected:
			void	FiberEntryPoint();

		// - TYPE PUBLISHING ---------------------------------

		private:
			enum class BarrierStatus : ::Eldritch2::uint32 {
				AwaitingTransfer,
				Complete,
				Available
			};

		// ---

			enum class ExecutionBehavior : ::Eldritch2::uint32 {
				Terminate,
				Continue
			};

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::ResizableArray<SuspendedContext>	_suspendedContexts;
			::Eldritch2::ResizableArray<SystemFiber>		_pooledFibers;

			::std::atomic<WorkerThread*>					_thiefCell;
			size_t											_randomWorkerSeed;
			::std::atomic<ExecutionBehavior>				_executionBehavior;
			::std::atomic<BarrierStatus>					_transferCell;

			SystemFiber										_initialFiber;
			SystemFiber										_counterWaitFiber;
			SystemFiber										_switchFiber;

			SystemFiber										_targetFiber;
			
			SuspendedContext								_waitingTaskStagingArea;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref FiberScheduler instance.
		FiberScheduler( const System::SystemInterface& systemInterface, ::Eldritch2::Allocator& allocator );
		
		//!	Destroys this @ref FiberScheduler instance.
		~FiberScheduler();

	// - THREAD SCHEDULING -------------------------------

		::Eldritch2::ErrorCode	Enqueue( Scheduler::Thread& thread ) override sealed;

	// ---------------------------------------------------

		void	Bootstrap( size_t totalWorkerCount, const Scheduler::WorkerContext::WorkItem& initialTask );

		void	FlagForShutdown() override sealed;

	// ---------------------------------------------------

		void	Backoff( BackoffContext& context ) override sealed;

	// ---------------------------------------------------

		void	SetIdealProcessorAffinityForCaller( const ProcessorAffinity affinity ) const;

		void	SetThreadPriorityForCaller( const ThreadPriority priority ) override sealed;

	// - SYNCHRONIZATION OBJECT ALLOCATION ---------------

		::Eldritch2::Result<Utility::ReaderWriterUserMutex>	AllocateReaderWriterUserMutex( ::Eldritch2::Allocator& allocator ) override sealed;

		::Eldritch2::Result<Utility::UserSemaphore>			AllocateSemaphore( ::Eldritch2::Allocator& allocator, const int initialCount, const int maximumCount ) override sealed;

		::Eldritch2::Result<Utility::UserMutex>				AllocateUserMutex( ::Eldritch2::Allocator& allocator ) override sealed;

	// ---------------------------------------------------

		WorkerThread&	GetRandomWorkerThread( const WorkerThread& executingWorker, size_t& randomSeed );

		size_t			GetMaximumTaskParallelism() const override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator					_allocator;
		//!	DO NOT MODIFY OUTSIDE OF @ref Bootstrap().
		::Eldritch2::ResizableArray<WorkerThread>	_workerThreads;
		const size_t								_syncObjectAlignmentInBytes;
		WorkerContext::FinishCounter				_dummyCounter;
	};

}	// namespace Win32
}	// namespace Scheduler
}	// namespace Eldritch2