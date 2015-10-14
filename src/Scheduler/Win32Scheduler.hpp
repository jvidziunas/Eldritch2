/*==================================================================*\
  Win32Scheduler.hpp
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
#include <Utility/Memory/Win32HeapAllocator.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Containers/Range.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Scheduler/Thread.hpp>
#include <Scheduler/Task.hpp>
//------------------------------------------------------------------//
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	Win32SystemInterface;
	}
}

namespace Eldritch2 {
namespace Scheduler {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scheduler		= ::Eldritch2::Scheduler;

// ---------------------------------------------------

	class Win32Scheduler : public Scheduler::TaskScheduler {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class WorkerThread : public Scheduler::Thread, public Scheduler::WorkerContext {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref WorkerThread instance.
			/*!	@param[in] scheduler The @ref Win32Scheduler instance that will own the new @ref WorkerThread.
				*/
			WorkerThread( Scheduler::Win32Scheduler& scheduler );

			//!	Destroys this @ref WorkerThread instance.
			~WorkerThread() = default;

		// ---------------------------------------------------

			const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Run() override sealed;

			void					RequestGracefulShutdown() override;

		// ---------------------------------------------------

		protected:
			void	AcquireReceiverInitiatedStolenTask( Scheduler::Win32Scheduler& scheduler );

		// - TYPE PUBLISHING ---------------------------------

		private:
			enum class BarrierStatus : ::Eldritch2::uint32 {
				AWAITING_TRANSFER,
				COMPLETE,
				AVAILABLE
			};

		// ---------------------------------------------------

			void	TryCompleteWorkSharingRequest();

		// - DATA MEMBERS ------------------------------------

			::std::atomic<Win32Scheduler::WorkerThread*>	_stealingWorker;
			size_t											_randomWorkerSeed;
			::std::atomic<::Eldritch2::uint32>				_killFlag;
			::std::atomic<BarrierStatus>					_transferBarrier;
		};

	// ---

		typedef ::Eldritch2::uint64	ProcessorAffinity;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref Win32Scheduler instance.
		Win32Scheduler( const Foundation::Win32SystemInterface& systemInterface, ::Eldritch2::Allocator& allocator );
		
		//!	Destroys this @ref Win32Scheduler instance.
		~Win32Scheduler();

	// - THREAD SCHEDULING -------------------------------

		::Eldritch2::ErrorCode	Enqueue( Scheduler::Thread& thread ) override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Bootstrap( Scheduler::Task& initialTask, size_t totalWorkerCount );

		void					FlagForShutdown() override sealed;

	// ---------------------------------------------------

		void	Backoff( TaskScheduler::BackoffContext& context ) override sealed;

		void	Sleep( const ::Eldritch2::uint64 timeInMicroseconds ) override sealed;

	// ---------------------------------------------------

		void	SetIdealProcessorAffinityForCaller( const ProcessorAffinity affinity ) const;

		void	SetThreadPriorityForCaller( const ThreadPriority priority ) override sealed;

		void	SetThreadNameForCaller( const char* const threadName ) const;

	// - SYNCHRONIZATION OBJECT ALLOCATION/DEALLOCATION --

		Utility::Result<Utility::ReaderWriterUserMutex>	AllocateReaderWriterUserMutex( ::Eldritch2::Allocator& allocator ) override sealed;

		Utility::Result<Utility::WaitableUserEvent>		AllocateWaitableEvent( ::Eldritch2::Allocator& allocator, const EventInitialState initialState ) override sealed;

		Utility::Result<Utility::UserSemaphore>			AllocateSemaphore( ::Eldritch2::Allocator& allocator, const size_t initialCount, const size_t maximumCount ) override sealed;

		Utility::Result<Utility::UserMutex>				AllocateUserMutex( ::Eldritch2::Allocator& allocator ) override sealed;

	// ---------------------------------------------------

		WorkerThread&	GetRandomWorkerThread( const WorkerThread& executingWorker, size_t& randomSeed );

		size_t			GetMaximumTaskParallelism() const override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	ConvertCallerToThread( Scheduler::Thread& thread );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator			_allocator;
		const size_t						_systemCacheLineSizeInBytes;
		::Eldritch2::Range<WorkerThread*>	_workerThreads;
	};
	
}	// namespace Scheduler
	
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduler/Win32Scheduler.inl>
//------------------------------------------------------------------//