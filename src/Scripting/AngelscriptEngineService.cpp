/*==================================================================*\
  AngelscriptEngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Scripting/Angelscript/AngelscriptBytecodePackageView.hpp>
#include <Scripting/Angelscript/AngelscriptObjectGraphView.hpp>
#include <Scripting/Angelscript/AngelscriptNativeBindings.hpp>
#include <Scripting/Angelscript/AngelscriptWorldView.hpp>
#include <Configuration/ConfigurationDatabase.hpp>
#include <Scripting/AngelscriptEngineService.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//
#include <memory>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if( ET_DEBUG_MODE_ENABLED && ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
	ET_LINK_LIBRARY( "angelscript64d.lib" )
#elif( ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
	ET_LINK_LIBRARY( "angelscript64.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4706 )
#endif

namespace Eldritch2 {
namespace Scripting {

	void AngelscriptEngineService::EngineDeleter::operator()( ::asIScriptEngine* const scriptEngine ) {
		scriptEngine->ShutDownAndRelease();
	}

// ---------------------------------------------------

	AngelscriptEngineService::AngelscriptEngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _allocator( GetEngineAllocator(), UTF8L("Angelscript Engine Allocator") ), _scriptEngine( nullptr ) {}

// ---------------------------------------------------

	const UTF8Char* const AngelscriptEngineService::GetName() const {
		return UTF8L("Angelscript Virtual Machine");
	}

// ---------------------------------------------------

	void AngelscriptEngineService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof( AngelscriptWorldView ), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) AngelscriptWorldView( world, static_cast<AngelscriptEngineService*>(parameter)->GetScriptEngine() ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void AngelscriptEngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		AngelscriptWorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void AngelscriptEngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using Initializer		= ResourceView::Initializer;

	// ---

		// Bytecode package
		visitor.PublishFactory( AngelscriptBytecodePackageView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* parameter ) -> Result<ResourceView> {
			unique_ptr<AngelscriptBytecodePackageView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) AngelscriptBytecodePackageView( initializer, allocator ), { allocator } );
					
			// Try to load from the source asset data.
			if( view && view->InstantiateFromByteArray( initializer.serializedAsset, static_cast<AngelscriptEngineService*>(parameter)->GetScriptEngine() ) ) {
				return { *view.release() };
			}

			// If we were able to allocate a view, then something was wrong with the source data. If we didn't get that far, the allocator ran out of resources.
			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} )
		// Object graph
		.PublishFactory( AngelscriptObjectGraphView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* /*parameter*/ ) -> Result<ResourceView> {
			if( auto* const	view = new(allocator, AllocationOption::PERMANENT_ALLOCATION) AngelscriptObjectGraphView( initializer, allocator ) ) {
				return { *view };
			}

			return { Error::OUT_OF_MEMORY };
		} );
	}

// ---------------------------------------------------

	void AngelscriptEngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const InitializeEngineTaskVisitor ) {
		class ExposeScriptAPITask : public CRTPTransientTask<ExposeScriptAPITask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ExposeScriptAPITask instance.
			ETInlineHint ExposeScriptAPITask( AngelscriptEngineService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<ExposeScriptAPITask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

			//!	Destroys this @ref ExposeScriptAPITask instance.
			ETInlineHint ~ExposeScriptAPITask() = default;

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Expose Script API Task");
			}

		// ---------------------------------------------------

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.CreateScriptAPI();
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			AngelscriptEngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ExposeScriptAPITask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void AngelscriptEngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const ServiceTickTaskVisitor ) {
		class CollectScriptGarbageTask : public CRTPTransientTask<CollectScriptGarbageTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref CollectScriptGarbageTask instance.
			ETInlineHint CollectScriptGarbageTask( AngelscriptEngineService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<CollectScriptGarbageTask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Destroy Script Garbage Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host._scriptEngine->GarbageCollect( ::asGC_DETECT_GARBAGE | ::asGC_DESTROY_GARBAGE | ::asGC_ONE_STEP );
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			AngelscriptEngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) CollectScriptGarbageTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	StringMarshal AngelscriptEngineService::MarshalStringLiteral( const unsigned int literalLengthInOctets, const UTF8Char* const stringLiteral ) {
		return StringMarshal( stringLiteral, literalLengthInOctets, _allocator );
	}

// ---------------------------------------------------

	void AngelscriptEngineService::MessageCallback( const ::asSMessageInfo* messageInfo ) {
		LogMessageType	messageType( LogMessageType::MESSAGE );
		const char*		description( UTF8L("message") );
		
		switch( messageInfo->type ) {
			case asMSGTYPE_ERROR: {
				messageType = LogMessageType::ERROR;
				description = "error";
				break;
			}
			case asMSGTYPE_WARNING: {
				messageType = LogMessageType::WARNING;
				description = "warning";
				break;
			}
		}

		GetLogger( messageType )( UTF8L("Angelscript %s in '%s'[%i, %i]: %s.") ET_UTF8_NEWLINE_LITERAL, description, messageInfo->section, messageInfo->row, messageInfo->col, messageInfo->message );
	}

// ---------------------------------------------------

	void AngelscriptEngineService::CreateScriptAPI() {
		GetLogger()( UTF8L("Registering script API.") ET_UTF8_NEWLINE_LITERAL );

		if( decltype(_scriptEngine) scriptEngine { ::asCreateScriptEngine() } ) {
			scriptEngine->SetMessageCallback( ::asMETHOD( AngelscriptEngineService, MessageCallback ), this, ::asECallConvTypes::asCALL_THISCALL );

			{
				ScriptAPIRegistrationInitializationVisitor	registrationVisitor( *scriptEngine );
				
				// Register critical components first.
				StringMarshal::ExposeScriptAPI( registrationVisitor );
				Float4Marshal::ExposeScriptAPI( registrationVisitor );
				OrientationMarshal::ExposeScriptAPI( registrationVisitor );
				
				BroadcastInitializationVisitor( registrationVisitor );
			}

			RegisterCMathLibrary( scriptEngine.get() );
			RegisterAlgorithmLibrary( scriptEngine.get() );
			scriptEngine->RegisterStringFactory( StringMarshal::scriptTypeName, ::asMETHOD( AngelscriptEngineService, MarshalStringLiteral ), ::asECallConvTypes::asCALL_THISCALL_ASGLOBAL, this );
			// scriptEngine->SetContextCallbacks( asREQUESTCONTEXTFUNC_t requestCtx, asRETURNCONTEXTFUNC_t returnCtx, this );

			GetLogger()( UTF8L("Script API registered successfully.") ET_UTF8_NEWLINE_LITERAL );

			// Transfer ownership to the main engine.
			_scriptEngine = move( scriptEngine );
		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to create Angelscript SDK instance!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

}	// namespace Scripting
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif