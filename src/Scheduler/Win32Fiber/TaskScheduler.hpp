/*==================================================================*\
  TaskScheduler.hpp
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
#include <Utility/Memory/ChildAllocator.hpp>
#include <Scheduler/PrivateQueueWorker.hpp>
#include <Utility/Containers/Range.hpp>
#include <Scheduler/TaskScheduler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace System {
		class	SystemInterface;
	}
}

namespace Eldritch2 {
namespace Scheduler {
namespace Win32Fiber {

	class TaskScheduler : public Scheduler::TaskScheduler {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ProcessorAffinity	= ::Eldritch2::uint64;
		using WorkerThread		= Scheduler::PrivateQueueWorker<Win32Fiber::TaskScheduler>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref TaskScheduler instance.
		TaskScheduler( const System::SystemInterface& systemInterface, ::Eldritch2::Allocator& allocator );
		
		//!	Destroys this @ref TaskScheduler instance.
		~TaskScheduler();

	// - THREAD SCHEDULING -------------------------------

		::Eldritch2::ErrorCode	Enqueue( Scheduler::Thread& thread ) override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Bootstrap( Scheduler::Task& initialTask, size_t totalWorkerCount );

		void					FlagForShutdown() override sealed;

	// ---------------------------------------------------

		void	Backoff( BackoffContext& context ) override sealed;

		void	Sleep( const ::Eldritch2::uint64 timeInMicroseconds ) override sealed;

	// ---------------------------------------------------

		void	SetIdealProcessorAffinityForCaller( const ProcessorAffinity affinity ) const;

		void	SetThreadPriorityForCaller( const ThreadPriority priority ) override sealed;

		void	SetThreadNameForCaller( const char* const threadName ) const;

	// - SYNCHRONIZATION OBJECT ALLOCATION ---------------

		::Eldritch2::Result<Utility::ReaderWriterUserMutex>	AllocateReaderWriterUserMutex( ::Eldritch2::Allocator& allocator ) override sealed;

		::Eldritch2::Result<Utility::WaitableUserEvent>		AllocateWaitableEvent( ::Eldritch2::Allocator& allocator, const EventInitialState initialState ) override sealed;

		::Eldritch2::Result<Utility::UserSemaphore>			AllocateSemaphore( ::Eldritch2::Allocator& allocator, const size_t initialCount, const size_t maximumCount ) override sealed;

		::Eldritch2::Result<Utility::UserMutex>				AllocateUserMutex( ::Eldritch2::Allocator& allocator ) override sealed;

	// ---------------------------------------------------

		WorkerThread&	GetRandomWorkerThread( const WorkerThread& executingWorker, size_t& randomSeed );

		size_t			GetMaximumTaskParallelism() const override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	ConvertCallerToThread( Scheduler::Thread& thread );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator			_allocator;
		const size_t						_syncObjectAlignmentInBytes;
		::Eldritch2::Range<WorkerThread*>	_workerThreads;
	};

}	// namespace Win32
}	// namespace Scheduler
}	// namespace Eldritch2