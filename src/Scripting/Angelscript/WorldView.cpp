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
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
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

	void WorldView::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const FrameTickTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, []( void* view, WorkerContext& executingContext ) {
			{	WorkerContext::FinishCounter	preScriptTickFinishCounter( 0 );
				static_cast<WorldView*>(view)->BroadcastTaskVisitor( executingContext, preScriptTickFinishCounter, WorldView::PreScriptTickTaskVisitor() );

				executingContext.WaitForCounter( preScriptTickFinishCounter );
			}

			{	WorkerContext::FinishCounter	scriptTickFinishCounter( 0 );
				static_cast<WorldView*>(view)->BroadcastTaskVisitor( executingContext, scriptTickFinishCounter, WorldView::ScriptTickTaskVisitor() );

				executingContext.WaitForCounter( scriptTickFinishCounter );
			}

			{	WorkerContext::FinishCounter	scriptTickFinishCounter( 0 );
				static_cast<WorldView*>(view)->BroadcastTaskVisitor( executingContext, scriptTickFinishCounter, WorldView::PostScriptTickTaskVisitor() );

				executingContext.WaitForCounter( scriptTickFinishCounter );
			}
		} } );
	}

// ---------------------------------------------------

	void WorldView::AcceptTaskVisitor( WorkerContext& executingContext, WorkerContext::FinishCounter& finishCounter, const ScriptTickTaskVisitor ) {
		executingContext.Enqueue( finishCounter, { this, []( void* /*view*/, WorkerContext& /*executingContext*/ ) {
			// Dispatch script messages
		} } );
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