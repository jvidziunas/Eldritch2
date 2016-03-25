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
#include <Scripting/Angelscript/BytecodePackageResourceView.hpp>
#include <Scripting/Angelscript/ObjectGraphResourceView.hpp>
#include <Scripting/Angelscript/WorldView.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Scheduler/WorkerContext.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
#include <Logging/Logger.hpp>
#include <Utility/Assert.hpp>
#include <Build.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace {

	static ETThreadLocal AngelScript::WorldView*	activeScriptWorldView = nullptr;

	static const ::asITypeInfo*	TryLocateClass( const ContentLibrary& library, const UTF8Char* const classPath ) {
		FixedStackAllocator<64u>	tempAllocator( UTF8L("TryLocateClass() Temporary Allocator") );
		const auto					moduleName( FindFirstInstance( classPath, '@' ) );

		if( nullptr == moduleName ) {
			return nullptr;
		}

		const auto	package( library.ResolveViewByName<AngelScript::BytecodePackageResourceView>( moduleName + 1 ) );
		if( nullptr == package ) {
			return nullptr;
		}

		UTF8String<>	className( classPath, moduleName, { tempAllocator, UTF8L("Class Name Allocator")} );

		return package->GetScriptModule()->GetTypeInfoByName( className.AsCString() );
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	WorldView::WorldView( World& owningWorld, ::asIScriptEngine& scriptEngine ) : Foundation::WorldView( owningWorld ),
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
		activeScriptWorldView = this;
		StringMarshal::AttachWorldAllocator( _stringAllocator );
	}

// ---------------------------------------------------

	void WorldView::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& visitor ) {
		visitor.ExposeFunction<void, const StringMarshal&>( "Spawn", [] ( const StringMarshal& className ) {
			activeScriptWorldView->Spawn( TryLocateClass( activeScriptWorldView->GetContentLibrary(), className.AsCString() ) );
		} ).ExposeFunction<void, const StringMarshal&>( "RequestWorldShutdown", [] ( const StringMarshal& reason ) {
			activeScriptWorldView->GetOwningWorld().GetLogger( LogMessageType::Message )( UTF8L("Script has requested world shutdown: {}.") ET_UTF8_NEWLINE_LITERAL, reason.AsCString() );
			activeScriptWorldView->RequestWorldShutdown();
		} ).ExposeFunction<void>( "RequestWorldShutdown", [] () {
			activeScriptWorldView->GetOwningWorld().GetLogger( LogMessageType::Message )( UTF8L("Script has requested world shutdown.") ET_UTF8_NEWLINE_LITERAL );
			activeScriptWorldView->RequestWorldShutdown();
		} ).ExposeFunction<StringMarshal, const StringMarshal&>( "GetWorldProperty", [] ( const StringMarshal& propertyName ) -> StringMarshal {
			return { ::std::move( activeScriptWorldView->GetOwningWorld().GetPropertyByKey( activeScriptWorldView->GetWorldAllocator(), propertyName.AsCString() ) ) };
		} );
	}

// ---------------------------------------------------

	void WorldView::AcceptViewVisitor( const DeletionPreparationVisitor ) {
		// Prepare for script execution (this will take place within the various release methods called implicitly below)
		BroadcastViewVisitor( ScriptExecutionPreparationVisitor() );

		_rulesEntity.reset();
	}

// ---------------------------------------------------

	void WorldView::OnGameStart( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Process game start", 0xCDCDCD );

		const auto	rulesClassName( GetOwningWorld().GetPropertyByKey( _stringAllocator, World::GetRulesKey() ) );
		const auto	resourceObjectName( GetOwningWorld().GetPropertyByKey( _stringAllocator, UTF8L("Resource"), UTF8L("") ) );

		// Prepare to execute script code.
		BroadcastViewVisitor( ScriptExecutionPreparationVisitor() );

		if( !rulesClassName.IsEmpty() ) {
			auto	rulesEntity( Spawn( TryLocateClass( GetContentLibrary(), rulesClassName.AsCString() ) ) );

			if( !rulesEntity ) {
				GetOwningWorld().GetLogger( LogMessageType::Error )( UTF8L("Unable to create rules entity '{}'!") ET_UTF8_NEWLINE_LITERAL, rulesClassName.AsCString() );
				GetOwningWorld().RaiseFatalError();
			}
				
			_rulesEntity = ::std::move( rulesEntity );
		}

		if( !resourceObjectName.IsEmpty() ) {
			if( const auto view = GetContentLibrary().ResolveViewByName<ObjectGraphResourceView>( resourceObjectName.AsCString() ) ) {
				view->DeserializeIntoWorldView( *this );
			}
		}
	}

// ---------------------------------------------------

	ObjectHandle WorldView::Spawn( const ::asITypeInfo* typeInfo ) {
		if( !typeInfo || !(typeInfo->GetFlags() & ::asOBJ_SCRIPT_OBJECT) ) {
			return { nullptr };
		}

		return ObjectHandle( static_cast<::asIScriptObject*>( _scriptEngine.CreateScriptObject( typeInfo ) ) );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2