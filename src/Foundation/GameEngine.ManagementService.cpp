/*==================================================================*\
  GameEngine.ManagementService.cpp
  ------------------------------------------------------------------
  Purpose:
  Implements an 'engine' object that binds together most subsystems
  into a coherent whole. Also serves as an intermediate layer
  between higher-level OS-dependent application systems and
  low-level generic simulation engines.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/TaskScheduler.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Foundation/GameEngine.hpp>
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Foundation {

	GameEngine::ManagementService::ManagementService( GameEngine& owningEngine ) : GameEngineService( owningEngine ) {}

// ---------------------------------------------------

	const UTF8Char* const GameEngine::ManagementService::GetName() const {
		return UTF8L("Game Engine Management Service");
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::BootstrapEngine( const size_t threadCount ) {
		// Hooray for nesting!
		class TickFrameTask : public Task {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class TickWorldsTask : public Task {
			// - TYPE PUBLISHING ---------------------------------

			public:
				class TickServicesTask : public Task {
				// - CONSTRUCTOR/DESTRUCTOR --------------------------

				public:
					//! Constructs this @ref TickServicesTask instance.
					ETInlineHint TickServicesTask( ManagementService& host, TickWorldsTask& parent, WorkerContext& executingContext ) : Task( parent, Scheduler::ContinuationTaskSemantics ),
																																		_engine( host._owningEngine ),
																																		_frameAllocator( host.GetEngineAllocator(),
																									   													_engine->CalculateFrameArenaSizeInBytes(),
																									   													Allocator::AllocationOption::TEMPORARY_ALLOCATION,
																									   													UTF8L("Frame Temporary Allocator") ) {
						TrySchedulingOnContext( executingContext );
					}

					~TickServicesTask() = default;

				// ---------------------------------------------------

					ETInlineHint const ObjectHandle<GameEngine>& GetEngineReference() {
						return _engine;
					}

					ETInlineHint GameEngine& GetGameEngine() {
						return *_engine;
					}					

					ETInlineHint Allocator& GetFrameAllocator() {
						return _frameAllocator;
					}

				// ---------------------------------------------------

					const UTF8Char* const GetHumanReadableName() const override sealed {
						return UTF8L("Tick Engine Services Task");
					}

					Task* Execute( WorkerContext& executingContext ) override sealed {
						GetGameEngine()._managementService.BroadcastTaskVisitor( GetFrameAllocator(), *this, executingContext, GameEngineService::ServiceTickTaskVisitor() );
						return nullptr;
					}

				// - DATA MEMBERS ------------------------------------

				private:
					// This is responsible for keeping the engine alive for the duration of the task.
					ObjectHandle<GameEngine>	_engine;
					ArenaChildAllocator			_frameAllocator;
				};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				//!	Constructs this @ref TickWorldsTask instance.
				ETInlineHint TickWorldsTask( ManagementService& host, TickFrameTask& parent, WorkerContext& executingContext ) : Task( parent, Scheduler::CodependentTaskSemantics ),
																																 _tickServicesTask( host, *this, executingContext ) {
					TrySchedulingOnContext( executingContext );
				}

				~TickWorldsTask() = default;

			// ---------------------------------------------------

				ETInlineHint const ObjectHandle<GameEngine>& GetEngineReference() {
					return _tickServicesTask.GetEngineReference();
				}

				ETInlineHint GameEngine& GetGameEngine() {
					return _tickServicesTask.GetGameEngine();
				}

				ETInlineHint Allocator& GetFrameAllocator() {
					return _tickServicesTask.GetFrameAllocator();
				}

			// ---------------------------------------------------

				const UTF8Char* const GetHumanReadableName() const override sealed {
					return UTF8L("Tick Worlds Task");
				}

				Task* Execute( WorkerContext& executingContext ) override sealed {
					GetGameEngine()._managementService.BroadcastTaskVisitor( GetFrameAllocator(), *this, executingContext, GameEngineService::WorldTickTaskVisitor() );
					return nullptr;
				}

			// - DATA MEMBERS ------------------------------------

			private:
				TickServicesTask	_tickServicesTask;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref TickFrameTask instance.
			ETInlineHint TickFrameTask( ManagementService& host, WorkerContext& executingContext ) : _tickWorldsTask( host, *this, executingContext ) {
				TrySchedulingOnContext( executingContext );
			}

			~TickFrameTask() = default;

		// ---------------------------------------------------

			ETInlineHint const ObjectHandle<GameEngine>& GetEngineReference() {
				return _tickWorldsTask.GetEngineReference();
			}

			ETInlineHint ManagementService& GetManagementService() {
				return GetGameEngine()._managementService;
			}

			ETInlineHint GameEngine& GetGameEngine() {
				return _tickWorldsTask.GetGameEngine();
			}			

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Tick Frame Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				return nullptr;
			}

			void Finalize( WorkerContext& executingContext ) override sealed {
				auto&	engineAllocator( GetGameEngine()._allocator );

				::MicroProfileFlip( nullptr );

				if( !GetEngineReference().IsSoleReferenceToObject() ) {
					new(engineAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TickFrameTask( GetManagementService(), executingContext );
				}

				engineAllocator.Delete( *this );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			TickWorldsTask	_tickWorldsTask;
		};

	// ---

		class InitializeEngineTask : public Task {
		public:
			enum : size_t {
				ARENA_SIZE_IN_BYTES = 1024 * 1024 * 1024
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref InitializeEngineTask instance.
			InitializeEngineTask( ManagementService& service ) : _engineReference( service._owningEngine, ::Eldritch2::PassthroughReferenceCountingSemantics ),
																 _initializationAllocator( UTF8L("Engine Initialization Temporary Allocator") ) {}

		// ---------------------------------------------------

			ETInlineHint ManagementService& GetManagementService() {
				return _engineReference->_managementService;
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Initialize Engine Task");
			}

		// ---------------------------------------------------

			Task* Execute( WorkerContext& executingContext ) override sealed {
				GetManagementService().BroadcastTaskVisitor( _initializationAllocator, *this, executingContext, GameEngineService::InitializeEngineTaskVisitor() );

				return nullptr;
			}

			void Finalize( WorkerContext& executingContext ) override sealed {
				auto&	engineAllocator( _engineReference->_allocator );

				GetManagementService().BroadcastInitializationVisitor( GameEngineService::PostInitializationVisitor() );

				// Log a successful initialization and begin the frame loop.
				GetManagementService().GetLogger()( UTF8L("Engine initialization complete!") ET_UTF8_NEWLINE_LITERAL );

				if( !_engineReference.IsSoleReferenceToObject() ) {
					new(engineAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TickFrameTask( GetManagementService(), executingContext );
				}

				// We've finally rendered ourselves redundant.
				engineAllocator.Delete( *this );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			//! This is responsible for keeping the engine alive for the duration of the task.
			ObjectHandle<GameEngine>					_engineReference;
			FixedStackAllocator<ARENA_SIZE_IN_BYTES>	_initializationAllocator;
		};

	// ---

		if( auto initializeEngineTask = new(GetEngineAllocator(), Allocator::AllocationOption::TEMPORARY_ALLOCATION) InitializeEngineTask( *this ) ) {
			GetEngineTaskScheduler().Bootstrap( *initializeEngineTask, threadCount ).ToInt();
		}
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const InitializeEngineTaskVisitor ) {
		class PublishResourceViewFactoriesTask : public CRTPTransientTask<PublishResourceViewFactoriesTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PublishResourceViewFactoriesTask instance.
			ETInlineHint PublishResourceViewFactoriesTask( ManagementService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<PublishResourceViewFactoriesTask>( visitingTask, Scheduler::CodependentTaskSemantics ),
																																			_hostingService( host ) {
				TrySchedulingOnContext( executingContext );
			}

			~PublishResourceViewFactoriesTask() = default;

		// ---------------------------------------------------

			ETInlineHint ManagementService&	GetHost() {
				return _hostingService;
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Publish Service Resource View Factories Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				GetHost().BroadcastInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor( GetHost()._owningEngine._contentLibrary ) );

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			ManagementService&	_hostingService;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) PublishResourceViewFactoriesTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const WorldTickTaskVisitor ) {
		_owningEngine._tickingWorlds.ClearAndDispose( [&subtaskAllocator, &visitingTask, &executingContext] ( World& tickingWorld ) {
			tickingWorld.QueueUpdateTasks( subtaskAllocator, executingContext, visitingTask );
		} );
	}

// ---------------------------------------------------

	void GameEngine::ManagementService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("Engine") );

		visitor.Register( UTF8L("LogEchoThreshold"), _owningEngine._logEchoThreshold ).Register( UTF8L("TaskArenaPerThreadAllocationSizeInBytes"), _owningEngine._taskArenaPerThreadAllocationSizeInBytes );
	}

}	// namespace Foundation
}	// namespace Eldritch2