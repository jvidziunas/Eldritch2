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
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
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
#include <microprofile/microprofile.h>
#include <angelscript.h>
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

	void WorldView::OnFrameTick( WorkerContext& executingContext ) {
		InvokeTickFunction<&Foundation::WorldView::OnPreScriptTick>( executingContext );
		InvokeTickFunction<&Foundation::WorldView::OnScriptTick>( executingContext );
		InvokeTickFunction<&Foundation::WorldView::OnPostScriptTick>( executingContext );
	}

// ---------------------------------------------------

	void WorldView::OnScriptTick( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Process script messages", 0xCDCDCD );
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) {
		StringMarshal::AttachWorldAllocator( _stringAllocator );
	}

// ---------------------------------------------------

	void WorldView::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& /*visitor*/ ) {}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const DeletionPreparationVisitor ) {
		// Prepare for script execution (this will take place within the various release methods called implicitly below)
		BroadcastViewVisitor( ScriptExecutionPreparationVisitor() );

		_rulesEntity.reset();
	}

// ---------------------------------------------------

	void WorldView::OnGameStart( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Process game start", 0xCDCDCD );

		const auto	rulesObjectName( GetOwningWorld().GetPropertyByKey( _stringAllocator, UTF8L("GameRulesClass"), UTF8L("") ) );
		const auto	resourceObjectName( GetOwningWorld().GetPropertyByKey( _stringAllocator, UTF8L("Resource"), UTF8L("") ) );

		if( !rulesObjectName.IsEmpty() ) {
			_rulesEntity = ::std::move( Spawn( rulesObjectName.AsCString() ) );
		}

		if( !resourceObjectName.IsEmpty() ) {
			if( const auto view = GetContentLibrary().ResolveViewByName<ObjectGraphResourceView>( resourceObjectName.AsCString() ) ) {
				view->DeserializeIntoWorldView( *this );
			}
		}
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