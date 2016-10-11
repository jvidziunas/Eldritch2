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
#include <Scripting/AngelScript/AssetViews/BytecodePackageView.hpp>
#include <Scripting/AngelScript/AssetViews/ObjectGraphView.hpp>
#include <Scripting/AngelScript/NativeBindings.hpp>
#include <Configuration/ConfigurationDatabase.hpp>
#include <Animation/ScriptComponents/Armature.hpp>
#include <Scripting/AngelScript/EngineService.hpp>
#include <Scripting/AngelScript/ApiRegistrar.hpp>
#include <Scripting/Angelscript/WorldService.hpp>
#include <Assets/AssetViewFactoryRegistrar.hpp>
#include <Scripting/AngelScript/ApiExport.hpp>
#include <Platform/SynchronousFileWriter.hpp>
#include <Platform/ContentProvider.hpp>
#include <Utility/ErrorCode.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
#include <angelscript.h>
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
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4706 )
#endif

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace {

	static ETPureFunctionHint ErrorCode ExportApi( Allocator& allocator, ContentProvider& contentProvider, asIScriptEngine& scriptEngine ) {
		Result<SynchronousFileWriter>	createWriterResult( contentProvider.CreateOrTruncateSynchronousFileWriter( WellKnownDirectory::WorkingDirectory, "Eldritch2.E2AngelscriptApi" ) );
		if( !createWriterResult ) {
			return createWriterResult.GetErrorCode();
		}

		return ExportScriptApi( allocator, scriptEngine, *createWriterResult );
	}

}	// anonymous namespace

	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ), _allocator( engine.GetAllocator(), "Angelscript Engine Allocator" ), _log( engine.GetLog() ), _forceExport( false ) {}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "Angelscript Virtual Machine";
	}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<Core::WorldService>> EngineService::CreateWorldService( Allocator& allocator, const World& world ) {
		auto	result( MakeUnique<AngelScript::WorldService>( allocator, world ) );

		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( Scripting::ApiRegistrar& registrar ) {
		WorldService::RegisterScriptApi( registrar );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& executor, const BeginInitializationVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Register script API", 0xBBBBBBBB );

		_log( MessageSeverity::Message, "Registering script API." ET_UTF8_NEWLINE_LITERAL );

		UniquePointer<asIScriptEngine>	scriptEngine( asCreateScriptEngine() );
		if( !scriptEngine ) {
			_log( MessageSeverity::Error, "Unable to create Angelscript SDK instance!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		scriptEngine->SetMessageCallback( asMETHOD(EngineService, MessageCallback), this, asECallConvTypes::asCALL_THISCALL );
		scriptEngine->SetContextCallbacks( [] ( asIScriptEngine*, void* engineService ) { return static_cast<EngineService*>(engineService)->GetContext(); },
										   [] ( asIScriptEngine*, asIScriptContext* context, void* engineService ) { static_cast<EngineService*>(engineService)->ReturnContext( context ); },
										   this );
	
		{	AngelScript::ApiRegistrar	registrar( *scriptEngine, _allocator );
		//	Register 'low-level' shared script types here, as we don't know when the main registration method will be invoked relative to other services.
			registrar.DefineCoreTypes();

			ScriptComponents::Armature::RegisterScriptApi( registrar );

		//	scriptEngine->RegisterDefaultArrayType();
			scriptEngine->RegisterStringFactory( registrar.GetTypeName( typeid(Utf8String<>) ), asMETHOD( EngineService, MarshalStringLiteral ), asECallConvTypes::asCALL_THISCALL_ASGLOBAL, this );
			
			LocateService<Engine>().VisitServices( registrar );
		}

		RegisterCMath( scriptEngine.Get() );
		RegisterAlgorithms( scriptEngine.Get() );

		_log( MessageSeverity::Message, "Script API registered successfully." ET_UTF8_NEWLINE_LITERAL );

	//	Transfer ownership.
		_scriptEngine = eastl::move( scriptEngine );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& executor, const InitializationCompleteVisitor ) {
		if( !_scriptEngine ) {
			return;
		}

	//	Export the script API for the compiler tool.
		const ErrorCode	exportApiResult( ExportApi( _allocator, LocateService<ContentProvider>(), *_scriptEngine ) );
		if( !exportApiResult ) {
			_log( MessageSeverity::Warning, "Unable to export script API: {}." ET_UTF8_NEWLINE_LITERAL, GetErrorString( exportApiResult ) );
			return;
		}

		_log( MessageSeverity::Message, "Exported script API successfully." ET_UTF8_NEWLINE_LITERAL );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ServiceBlackboard& serviceLocator ) {
		serviceLocator.Publish( *_scriptEngine );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( AssetViewFactoryRegistrar& registrar ) {
		registrar.Publish( AssetViews::BytecodePackageView::GetExtension(), [this] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
				return AssetViews::BytecodePackageView::CreateView( allocator, *_scriptEngine, library, name, rawBytes );
			} )
			.Publish( AssetViews::ObjectGraphView::GetExtension(), [] ( Allocator& allocator, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
				return AssetViews::ObjectGraphView::CreateView( allocator, library, name, rawBytes );
			} );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ServiceTickVisitor visitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Collect script garbage", 0xBBBBBBBB );

		_scriptEngine->GarbageCollect( asEGCFlags::asGC_DETECT_GARBAGE | asEGCFlags::asGC_DESTROY_GARBAGE );
	}

// ---------------------------------------------------

	Utf8String<> EngineService::MarshalStringLiteral( const unsigned int literalLengthInOctets, const Utf8Char* const stringLiteral ) {
		return { stringLiteral, stringLiteral + literalLengthInOctets, { _allocator, "Marshalled String Allocator" } };
	}

// ---------------------------------------------------

	void EngineService::MessageCallback( const asSMessageInfo* messageInfo ) {
		MessageSeverity	severity;
		const char*		description;
		
		switch( messageInfo->type ) {
			case asMSGTYPE_ERROR: {
				severity	= MessageSeverity::Error;
				description = "error";
				break;
			}
			case asMSGTYPE_WARNING: {
				severity	= MessageSeverity::Warning;
				description = "warning";
				break;
			}
			case asMSGTYPE_INFORMATION:
			default: {
				severity	= MessageSeverity::Message;
				description = "message";
				break;
			}
		}

		_log( severity, "Angelscript {} in '{}'[{}, {}]: {}." ET_UTF8_NEWLINE_LITERAL, description, messageInfo->section, messageInfo->row, messageInfo->col, messageInfo->message );
	}

// ---------------------------------------------------

	asIScriptContext* EngineService::GetContext() {
		return _scriptEngine->CreateContext();
	}

// ---------------------------------------------------

	void EngineService::ReturnContext( asIScriptContext* context ) {
		context->Release();
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif