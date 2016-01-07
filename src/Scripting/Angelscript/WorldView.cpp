/*==================================================================*\
  WorldView.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Scripting/Angelscript/ObjectGraphResourceView.hpp>
#include <Scripting/Angelscript/WorldView.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	static ETThreadLocal Scripting::AngelScript::WorldView*	activeScriptWorldView = nullptr;

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	WorldView::WorldView( Foundation::World& owningWorld, ::asIScriptEngine& scriptEngine ) : Foundation::WorldView( owningWorld ),
																							  _stringAllocator( { GetWorldAllocator(), UTF8L("World String Root Allocator") } ),
																							  _scriptEngine( (scriptEngine.AddRef(), scriptEngine) ) {}

// ---------------------------------------------------

	WorldView::~WorldView() {
		// Since scripts can create objects not directly owned by this WorldView, we need to make sure all shared resources have been released before we start destroying world views.
		// This should have been done within WorldView::AcceptViewVisitor( const DeletionPreparationVisitor ).

	// ---

		_scriptEngine.Release();
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const FrameTickTaskVisitor ) {
		// Holy nesting, Batman! The things we do to save making some copies of a pointer...
		class CollectPostScriptTickTasksTask : public Scheduler::CRTPTransientTask<CollectPostScriptTickTasksTask> {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class CollectScriptTickTasksTask : public Task {
			// - TYPE PUBLISHING ---------------------------------

			public:
				class CollectPreScriptTickTasksTask : public Task {
				// - CONSTRUCTOR/DESTRUCTOR --------------------------

				public:
					//!	Constructs this @ref CollectPreScriptTickTasksTask instance.
					ETInlineHint CollectPreScriptTickTasksTask( WorldView& host, Scheduler::WorkerContext& executingContext, CollectScriptTickTasksTask& parent, Allocator& subtaskAllocator ) : Task( parent, Scheduler::ContinuationTaskSemantics ),
																																																 _host( host ),
																																																 _subtaskAllocator( subtaskAllocator ) {
						TrySchedulingOnContext( executingContext );
					}

					~CollectPreScriptTickTasksTask() = default;

				// ---------------------------------------------------

					ETInlineHint WorldView&	GetHost() {
						return _host;
					}

					ETInlineHint Allocator& GetSubtaskAllocator() {
						return _subtaskAllocator;
					}

				// ---------------------------------------------------

					const UTF8Char* const GetHumanReadableName() const override sealed {
						return UTF8L("Angelscript Collect Pre-Script Tick Tasks Task");
					}

					Task* Execute( Scheduler::WorkerContext& executingContext ) override sealed {
						GetHost().BroadcastTaskVisitor( GetSubtaskAllocator(), executingContext, *this, WorldView::PreScriptTickTaskVisitor() );

						return nullptr;
					}

				// - DATA MEMBERS ------------------------------------

				private:
					WorldView&	_host;
					Allocator&	_subtaskAllocator;
				};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

				//!	Constructs this @ref CollectScriptTickTasksTask instance.
				ETInlineHint CollectScriptTickTasksTask( WorldView& host, Scheduler::WorkerContext& executingContext, CollectPostScriptTickTasksTask& parent, Allocator& subtaskAllocator ) : Task( parent, Scheduler::ContinuationTaskSemantics ),
																																															  _collectPreTickTasksTask( host, executingContext, *this, subtaskAllocator ) {
					TrySchedulingOnContext( executingContext );
				}

				~CollectScriptTickTasksTask() = default;

			// ---------------------------------------------------

				ETInlineHint WorldView&	GetHost() {
					return _collectPreTickTasksTask.GetHost();
				}

				ETInlineHint Allocator&	GetSubtaskAllocator() {
					return _collectPreTickTasksTask.GetSubtaskAllocator();
				}

			// ---------------------------------------------------

				const UTF8Char* const GetHumanReadableName() const override sealed {
					return UTF8L("Angelscript Collect Script Tick Tasks Task");
				}

				Task* Execute( Scheduler::WorkerContext& executingContext ) override sealed {
					GetHost().BroadcastTaskVisitor( GetSubtaskAllocator(), executingContext, *this, WorldView::ScriptTickTaskVisitor() );

					return nullptr;
				}
				
			// - DATA MEMBERS ------------------------------------

			private:
				CollectPreScriptTickTasksTask	_collectPreTickTasksTask;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref CollectPostScriptTickTasksTask instance.
			ETInlineHint CollectPostScriptTickTasksTask( WorldView& host, Scheduler::WorkerContext& executingContext, Task& tickTask, Allocator& subtaskAllocator ) : CRTPTransientTask<CollectPostScriptTickTasksTask>( tickTask, Scheduler::CodependentTaskSemantics ),
																																									  _collectScriptTickTasksTask( host, executingContext, *this, subtaskAllocator ) {
				TrySchedulingOnContext( executingContext );
			}

			~CollectPostScriptTickTasksTask() = default;
			
		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Angelscript Collect Post-Script Tick Tasks Task");
			}

			Task* Execute( Scheduler::WorkerContext& executingContext ) override sealed {
				_collectScriptTickTasksTask.GetHost().BroadcastTaskVisitor( _collectScriptTickTasksTask.GetSubtaskAllocator(), executingContext, *this, WorldView::PostScriptTickTaskVisitor() );

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			CollectScriptTickTasksTask	_collectScriptTickTasksTask;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) CollectPostScriptTickTasksTask( *this, executingContext, visitingTask, subtaskAllocator );
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const ScriptTickTaskVisitor ) {
		class TickScriptsTask : public Scheduler::CRTPTransientTask<TickScriptsTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref TickScriptsTask instance.
			ETInlineHint TickScriptsTask( WorldView& host, Scheduler::WorkerContext& executingContext, Task& tickTask ) : CRTPTransientTask<TickScriptsTask>( tickTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

			~TickScriptsTask() = default;
			
		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Angelscript Update World Scripts Task");
			}

			Task* Execute( Scheduler::WorkerContext& /*executingContext*/ ) override sealed {
				if( _host.GetOwningWorld().IsPaused() ) {
					return nullptr;
				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			WorldView&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TickScriptsTask( *this, executingContext, visitingTask );
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const LoadFinalizationVisitor ) {
		FixedStackAllocator<128u>	tempAllocator( UTF8L("WorldView::AcceptViewVisitor() Temporary Allocator") );
		const auto					allocationCheckpoint( tempAllocator.CreateCheckpoint() );

		{	const auto	rulesClassName( GetOwningWorld().GetPropertyByKey( tempAllocator, UTF8L("GameRules") ) );
			_rulesEntity = ::std::move( Spawn( rulesClassName.GetCharacterArray() ) );
		}

		// Rewind the allocation to conserve/recycle stack space. We don't need both strings at once.
		tempAllocator.RestoreCheckpoint( allocationCheckpoint );

		{	const auto	resourceName( GetOwningWorld().GetPropertyByKey( tempAllocator, UTF8L("ResourceName") ) );

			if( !resourceName.Empty() ) {
				GetEngineContentLibrary().ResolveViewByName( resourceName.GetCharacterArray(), *static_cast<ObjectGraphResourceView*>(nullptr) ).DeserializeIntoWorldView( *this );
			}
		}
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		StringMarshal::AttachWorldAllocator( _stringAllocator );
	}

// ---------------------------------------------------

	void WorldView::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		auto	Spawn( [] ( const StringMarshal& className ) {
			if( activeScriptWorldView->Spawn( className.GetCharacterArray() ) ) {

			} else {
				::asGetActiveContext()->SetException( "Invalid class name for Spawn()!" );
			}
		} );

		visitor.ExposeFunction( "Spawn", static_cast<void (ETScriptAPICall *)( const StringMarshal& )>(Spawn) );
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const DeletionPreparationVisitor ) {
		// Prepare for script execution (this will take place within the various release methods called implicitly below)
		BroadcastViewVisitor( ScriptExecutionPreparationVisitor() );

		_rulesEntity.reset();
	}

// ---------------------------------------------------

	AngelScript::ObjectHandle WorldView::Spawn( const char* const className ) {
		const auto	objectType( _scriptEngine.GetTypeInfoByName( className ) );

		if( objectType && (objectType->GetFlags() & ::asOBJ_SCRIPT_OBJECT) ) {
			return { static_cast<::asIScriptObject*>(_scriptEngine.CreateScriptObject( objectType )), {} };
		}

		return { nullptr };
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2