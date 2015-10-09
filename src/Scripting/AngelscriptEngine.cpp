/*==================================================================*\
  AngelscriptEngine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Scripting/AngelscriptEngine.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/MessagePackReader.hpp>
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
#if( ET_DEBUG_MODE_ENABLED )
#if( ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
ET_LINK_LIBRARY( "angelscript64d.lib" )
#endif
#else
#if( ET_PLATFORM_X86 && ET_PLATFORM_64BIT )
ET_LINK_LIBRARY( "angelscript64.lib" )
#endif
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

	AngelscriptEngine::AngelscriptEngine( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _allocator( GetEngineAllocator(), UTF8L("Angelscript Engine Allocator") ), _scriptEngine( nullptr ) {}

// ---------------------------------------------------

	AngelscriptEngine::~AngelscriptEngine() {
		if( _scriptEngine ) {
			_scriptEngine->ShutDownAndRelease();
		}
	}

// ---------------------------------------------------

	const UTF8Char* const AngelscriptEngine::GetName() const {
		return UTF8L("Angelscript Virtual Machine");
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof( AngelscriptWorldView ), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) AngelscriptWorldView( world, static_cast<AngelscriptEngine*>(parameter)->GetScriptEngine() ) ? Errors::NONE : Errors::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		StringMarshal::ExposeScriptAPI( visitor );
		Float4Marshal::ExposeScriptAPI( visitor );
		OrientationMarshal::ExposeScriptAPI( visitor );
		AngelscriptWorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using Initializer		= ResourceView::Initializer;

	// ---

		// Bytecode package
		visitor.PublishFactory( AngelscriptBytecodePackageView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* parameter ) -> ResultPair<ResourceView> {
			auto&	scriptEngine( static_cast<AngelscriptEngine*>(parameter)->GetScriptEngine() );

			if( unique_ptr<::asIScriptModule> module { scriptEngine.GetModule( initializer.name.first, asGM_ALWAYS_CREATE ) } ) {
				auto* const view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) AngelscriptBytecodePackageView( move( module ), initializer, allocator ) );
				
				if( !view ) {
					return { nullptr, Errors::OUT_OF_MEMORY };
				}

				if( MessagePackReader( initializer.serializedAsset )(*view) ) {
					return { view, Errors::NONE };
				}
					
				allocator.Delete( *view );
				return { nullptr, Errors::INVALID_PARAMETER };
			}

			return { nullptr, Errors::UNSPECIFIED };
		} )
		// Object graph
		.PublishFactory( AngelscriptObjectGraphView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* /*parameter*/ ) -> ResultPair<ResourceView> {
			auto* const	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) AngelscriptObjectGraphView( initializer, allocator ) );

			return { view, view ? Errors::NONE : Errors::OUT_OF_MEMORY };
		} );
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const InitializeEngineTaskVisitor ) {
		class ExposeScriptAPITask : public CRTPTransientTask<ExposeScriptAPITask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ExposeScriptAPITask instance.
			ETInlineHint ExposeScriptAPITask( AngelscriptEngine& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<ExposeScriptAPITask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
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
			AngelscriptEngine&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) ExposeScriptAPITask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const ServiceTickTaskVisitor ) {
		class CollectScriptGarbageTask : public CRTPTransientTask<CollectScriptGarbageTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref CollectScriptGarbageTask instance.
			ETInlineHint CollectScriptGarbageTask( AngelscriptEngine& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<CollectScriptGarbageTask>( visitingTask, Scheduler::CodependentTaskSemantics ), _host( host ) {
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
			AngelscriptEngine&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) CollectScriptGarbageTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	StringMarshal AngelscriptEngine::MarshalStringLiteral( const unsigned int literalLengthInOctets, const UTF8Char* const stringLiteral ) {
		return StringMarshal( stringLiteral, literalLengthInOctets, _allocator );
	}

// ---------------------------------------------------

	void AngelscriptEngine::MessageCallback( const ::asSMessageInfo* messageInfo ) {
		switch( messageInfo->type ) {
			case asMSGTYPE_ERROR: {
				FormatAndLogError( UTF8L("Angelscript error in '%s'[%i, %i]: %s!") ET_UTF8_NEWLINE_LITERAL, messageInfo->section, messageInfo->row, messageInfo->col, messageInfo->message );
				break;
			}
			case asMSGTYPE_WARNING: {
				FormatAndLogWarning( UTF8L("Angelscript warning in '%s'[%i, %i]: %s.") ET_UTF8_NEWLINE_LITERAL, messageInfo->section, messageInfo->row, messageInfo->col, messageInfo->message );
				break;
			}
			case asMSGTYPE_INFORMATION: {
				FormatAndLogString( UTF8L("Angelscript message in '%s'[%i, %i]: %s.") ET_UTF8_NEWLINE_LITERAL, messageInfo->section, messageInfo->row, messageInfo->col, messageInfo->message );
				break;
			}
		}
	}

// ---------------------------------------------------

	void AngelscriptEngine::CreateScriptAPI() {
		FormatAndLogString( UTF8L("Registering script API.") ET_UTF8_NEWLINE_LITERAL );

		if( ::asIScriptEngine* const scriptEngine = ::asCreateScriptEngine( ANGELSCRIPT_VERSION ) ) {
			_scriptEngine = scriptEngine;

			scriptEngine->SetMessageCallback( ::asMETHOD( AngelscriptEngine, MessageCallback ), this, ::asECallConvTypes::asCALL_THISCALL );
			// scriptEngine->SetContextCallbacks( asREQUESTCONTEXTFUNC_t requestCtx, asRETURNCONTEXTFUNC_t returnCtx, this );

			BroadcastInitializationVisitor( ScriptAPIRegistrationInitializationVisitor( *scriptEngine ) );

			RegisterCMathLibrary( scriptEngine );
			RegisterAlgorithmLibrary( scriptEngine );
			scriptEngine->RegisterStringFactory( StringMarshal::scriptTypeName, ::asMETHOD( AngelscriptEngine, MarshalStringLiteral ), ::asECallConvTypes::asCALL_THISCALL_ASGLOBAL, this );

			FormatAndLogString( UTF8L("Script API registered successfully.") ET_UTF8_NEWLINE_LITERAL );
		} else {
			FormatAndLogError( UTF8L("Unable to create Angelscript SDK instance!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

}	// namespace Scripting
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif