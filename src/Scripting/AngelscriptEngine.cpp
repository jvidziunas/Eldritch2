/*==================================================================*\
  AngelscriptEngine.cpp
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
#include <Utility/Memory/InstanceDeleters.hpp>
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

	void AngelscriptEngine::EngineDeleter::operator()( ::asIScriptEngine* const scriptEngine ) {
		scriptEngine->ShutDownAndRelease();
	}

// ---------------------------------------------------

	AngelscriptEngine::AngelscriptEngine( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _allocator( GetEngineAllocator(), UTF8L("Angelscript Engine Allocator") ), _scriptEngine( nullptr ) {}

// ---------------------------------------------------

	const UTF8Char* const AngelscriptEngine::GetName() const {
		return UTF8L("Angelscript Virtual Machine");
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( this, sizeof( AngelscriptWorldView ), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			return new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) AngelscriptWorldView( world, static_cast<AngelscriptEngine*>(parameter)->GetScriptEngine() ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		AngelscriptWorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void AngelscriptEngine::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using Initializer		= ResourceView::Initializer;

	// ---

		// Bytecode package
		visitor.PublishFactory( AngelscriptBytecodePackageView::GetSerializedDataTag(), _scriptEngine.get(), [] ( Allocator& allocator, const Initializer& initializer, void* parameter ) -> Result<ResourceView> {
			using View = AngelscriptBytecodePackageView;

		// ---

			const ExternalArenaAllocator::SizeType	stringSize( initializer.name.Size() + 1 );

			// We'll need to null-terminate the name, so create a temporary string on the stack.
			UTF8String<ExternalArenaAllocator>	moduleName( initializer.name.first, initializer.name.onePastLast, { _alloca( stringSize ), stringSize, UTF8L( "Module Name String Allocator" ) } );
			
			// First, we'll need an Angelscript module. The unique_ptr will take care of releasing resources in the event something pukes.
			if( unique_ptr<::asIScriptModule> module { static_cast<::asIScriptEngine*>(parameter)->GetModule( moduleName.GetCharacterArray(), asGM_ALWAYS_CREATE ) } ) {
				
				// Next, allocate the returned resource view object. Once again, the unique_ptr will handle deleting in the event this explodes.
				if( unique_ptr<View, InstanceDeleter> view { new(allocator, AllocationOption::PERMANENT_ALLOCATION) View( move( module ), initializer, allocator ), { allocator } } ) {
					
					// Finally, try to load from the source asset data.
					if( view->SerializeAndBindToModule( MessagePackReader( initializer.serializedAsset ) ) ) {
						return { *view.release() };
					}

					return { Error::INVALID_PARAMETER };
				}

				return { Error::OUT_OF_MEMORY };
			}

			return { Error::UNSPECIFIED };
		} )
		// Object graph
		.PublishFactory( AngelscriptObjectGraphView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* /*parameter*/ ) -> Result<ResourceView> {
			using View = AngelscriptObjectGraphView;

		// ---

			if( auto* const	view = new(allocator, AllocationOption::PERMANENT_ALLOCATION) View( initializer, allocator ) ) {
				return { *view };
			}

			return { Error::OUT_OF_MEMORY };
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

		if( decltype(_scriptEngine) scriptEngine { ::asCreateScriptEngine( ANGELSCRIPT_VERSION ) } ) {
			scriptEngine->SetMessageCallback( ::asMETHOD( AngelscriptEngine, MessageCallback ), this, ::asECallConvTypes::asCALL_THISCALL );

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
			scriptEngine->RegisterStringFactory( StringMarshal::scriptTypeName, ::asMETHOD( AngelscriptEngine, MarshalStringLiteral ), ::asECallConvTypes::asCALL_THISCALL_ASGLOBAL, this );
			// scriptEngine->SetContextCallbacks( asREQUESTCONTEXTFUNC_t requestCtx, asRETURNCONTEXTFUNC_t returnCtx, this );

			FormatAndLogString( UTF8L( "Script API registered successfully." ) ET_UTF8_NEWLINE_LITERAL );

			// Transfer ownership to the main engine.
			_scriptEngine = move( scriptEngine );
		} else {
			FormatAndLogError( UTF8L("Unable to create Angelscript SDK instance!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

}	// namespace Scripting
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif