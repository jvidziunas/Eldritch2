/*==================================================================*\
  EngineService.cpp
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
#include <Scripting/AngelScript/BytecodePackageResourceView.hpp>
#include <Scripting/AngelScript/ObjectGraphResourceView.hpp>
#include <Scripting/AngelScript/NativeBindings.hpp>
#include <Configuration/ConfigurationDatabase.hpp>
#include <Scripting/AngelScript/EngineService.hpp>
#include <Scripting/Angelscript/WorldView.hpp>
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
namespace AngelScript {

	void EngineService::EngineDeleter::operator()( ::asIScriptEngine* const scriptEngine ) {
		scriptEngine->ShutDownAndRelease();
	}

// ---------------------------------------------------

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _allocator( GetEngineAllocator(), UTF8L("Angelscript Engine Allocator") ), _scriptEngine( nullptr ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Angelscript Virtual Machine");
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof(WorldView), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) WorldView( world, static_cast<EngineService*>(parameter)->GetScriptEngine() ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		WorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using FactoryResult		= ResourceViewFactoryPublishingInitializationVisitor::FactoryResult;

	// ---

		// Bytecode package
		visitor.PublishFactory( BytecodePackageResourceView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const UTF8Char* const name, void* parameter ) -> FactoryResult {
			return { new(allocator, AllocationOption::PERMANENT_ALLOCATION) BytecodePackageResourceView( static_cast<EngineService*>(parameter)->GetScriptEngine(), name, allocator ), { allocator } };
		} )
		// Object graph
		.PublishFactory( ObjectGraphResourceView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const UTF8Char* const name, void* /*parameter*/ ) -> FactoryResult {
			return { new(allocator, AllocationOption::PERMANENT_ALLOCATION) ObjectGraphResourceView( name, allocator ), { allocator } };
		} );
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const InitializeEngineTaskVisitor ) {
		class ExposeScriptAPITask : public CRTPTransientTask<ExposeScriptAPITask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ExposeScriptAPITask instance.
			ETInlineHint ExposeScriptAPITask( EngineService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<ExposeScriptAPITask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
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
			EngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ExposeScriptAPITask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void EngineService::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const ServiceTickTaskVisitor ) {
		class CollectScriptGarbageTask : public CRTPTransientTask<CollectScriptGarbageTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref CollectScriptGarbageTask instance.
			ETInlineHint CollectScriptGarbageTask( EngineService& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<CollectScriptGarbageTask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
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
			EngineService&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) CollectScriptGarbageTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	StringMarshal EngineService::MarshalStringLiteral( const unsigned int literalLengthInOctets, const UTF8Char* const stringLiteral ) {
		return StringMarshal( stringLiteral, literalLengthInOctets, _allocator );
	}

// ---------------------------------------------------

	void EngineService::MessageCallback( const ::asSMessageInfo* messageInfo ) {
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

	void EngineService::CreateScriptAPI() {
		GetLogger()( UTF8L("Registering script API.") ET_UTF8_NEWLINE_LITERAL );

		if( decltype(_scriptEngine) scriptEngine { ::asCreateScriptEngine() } ) {
			scriptEngine->SetMessageCallback( ::asMETHOD( EngineService, MessageCallback ), this, ::asECallConvTypes::asCALL_THISCALL );

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
			scriptEngine->RegisterStringFactory( StringMarshal::scriptTypeName, ::asMETHOD( EngineService, MarshalStringLiteral ), ::asECallConvTypes::asCALL_THISCALL_ASGLOBAL, this );
			// scriptEngine->SetContextCallbacks( asREQUESTCONTEXTFUNC_t requestCtx, asRETURNCONTEXTFUNC_t returnCtx, this );

			GetLogger()( UTF8L("Script API registered successfully.") ET_UTF8_NEWLINE_LITERAL );

			// Transfer ownership to the main engine.
			_scriptEngine = move( scriptEngine );
		} else {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to create Angelscript SDK instance!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif