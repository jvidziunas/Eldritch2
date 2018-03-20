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

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {

	class FiberJobSystem : public Scheduling::JobSystem {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ProcessorAffinity	= uint64;
		using PlatformFiber		= void*;

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
			JobThread( FiberJobSystem& owner );
		//!	Constructs this @ref JobThread instance.
			JobThread( const JobThread& );

			~JobThread();

		// ---------------------------------------------------

		public:
			Utf8Literal	GetName() const override sealed;

		// ---------------------------------------------------

		public:
			void	Run() override sealed;

			void	SetShouldShutDown() override sealed;

		// ---------------------------------------------------

		public:
			bool	BeginShareWith( JobThread& thief );

			bool	ShouldAwaitTransfer() const;

		// ---------------------------------------------------

		protected:
			void	FiberEntryPoint() override;

		// - DATA MEMBERS ------------------------------------

		private:
			FiberJobSystem*				_owner;
			std::atomic<JobThread*>		_thief;
			size_t						_victimSeed;
			std::atomic<RunBehavior>	_runBehavior;
			std::atomic<TransferState>	_transferCell;
			JobThread*					_victim;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		FiberJobSystem( const FiberJobSystem& ) = delete;
	//!	Constructs this @ref FiberJobSystem instance.
		FiberJobSystem();
		
		~FiberJobSystem();

	// - THREAD SCHEDULING -------------------------------

	public:
		ErrorCode	LaunchOnCaller( Scheduling::Thread& thread ) override sealed;

		ErrorCode	Launch( Scheduling::Thread& thread ) override sealed;

	// ---------------------------------------------------

	public:
		template <typename WorkItem>
		int		Boot( size_t workerCount, WorkItem&& initialTask );

		void	SetShouldShutDown( int shutdownCode ) override sealed;

	// ---------------------------------------------------

	public:
		void	BackOff( BackoffContext& context ) override sealed;

	// ---------------------------------------------------

	public:
		void	SetCallerCoreAffinity( ProcessorAffinity affinity ) const;

		void	SetCallerExecutionPriority( ThreadExecutionPriority priority ) override sealed;

	// ---------------------------------------------------

	public:
		JobThread&	FindVictim( size_t& victimSeed );

		size_t		GetMaximumParallelism() const override sealed;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		FiberJobSystem&	operator=( const FiberJobSystem& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable MallocAllocator		_allocator;
	/*!	Collection of worker threads this scheduler instance uses to execute job closures.
	 *	DO NOT MODIFY OUTSIDE OF @ref Bootstrap(). */
		UniquePointer<JobThread[]>	_workers;
	/*!	Dummy barrier used as a target for the default (work-steal) task. The value is not
		guaranteed to reach 0 at consistent points (or even at all) so waiting on this barrier
		is explicitly forbidden. */
		JobFence					_dummy;
	/*!	Container for the application return code.
		@see @ref FiberJobSystem::ShutDown */
		std::atomic<int>			_shutdownCode;
	};

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.inl>
//------------------------------------------------------------------//