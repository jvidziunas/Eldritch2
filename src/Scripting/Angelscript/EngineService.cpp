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
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Scripting/AngelScript/BytecodePackageResourceView.hpp>
#include <Scripting/AngelScript/ObjectGraphResourceView.hpp>
#include <Scripting/AngelScript/NativeBindings.hpp>
#include <Configuration/ConfigurationDatabase.hpp>
#include <Scripting/AngelScript/EngineService.hpp>
#include <Scripting/Angelscript/WorldView.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
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

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _allocator( GetEngineAllocator(), UTF8L("Angelscript Engine Allocator") ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Angelscript Virtual Machine");
	}

// ---------------------------------------------------

	ErrorCode EngineService::AllocateWorldView( Allocator& allocator, World& world ) {
		return new(allocator, Allocator::AllocationDuration::Normal) WorldView( world, GetScriptEngine() ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		WorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void EngineService::OnEngineInitializationStarted( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "Angelscript Virtual Machine", "Create script API", 0xBBBBBBBB );

		GetLogger()( UTF8L("Registering script API.") ET_UTF8_NEWLINE_LITERAL );

		AngelScript::EngineHandle	scriptEngine( ::asCreateScriptEngine() );

		if( !scriptEngine ) {
			GetLogger( LogMessageType::Error )( UTF8L("Unable to create Angelscript SDK instance!") ET_UTF8_NEWLINE_LITERAL );

			return;
		}

		scriptEngine->SetMessageCallback( ::asMETHOD( EngineService, MessageCallback ), this, ::asECallConvTypes::asCALL_THISCALL );
		// scriptEngine->SetContextCallbacks( asREQUESTCONTEXTFUNC_t requestCtx, asRETURNCONTEXTFUNC_t returnCtx, this );

		// Register 'low-level' shared script types here, as we don't know when the main registration method will be invoked relative to other services.
		{	ScriptAPIRegistrationInitializationVisitor	registrationVisitor( *scriptEngine );
			
			StringMarshal::ExposeScriptAPI( registrationVisitor );
			Float4Marshal::ExposeScriptAPI( registrationVisitor );
			OrientationMarshal::ExposeScriptAPI( registrationVisitor );
			
			BroadcastInitializationVisitor( registrationVisitor );
		}

		RegisterCMathLibrary( scriptEngine.get() );
		RegisterAlgorithmLibrary( scriptEngine.get() );

		scriptEngine->RegisterStringFactory( StringMarshal::scriptTypeName, ::asMETHOD( EngineService, MarshalStringLiteral ), ::asECallConvTypes::asCALL_THISCALL_ASGLOBAL, this );

		GetLogger()( UTF8L("Script API registered successfully.") ET_UTF8_NEWLINE_LITERAL );

		// Transfer ownership.
		_bytecodePackageFactory.SetScriptEngine( ::std::move( scriptEngine ) );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( BytecodePackageResourceView::GetSerializedDataTag(), _bytecodePackageFactory ).PublishFactory( ObjectGraphResourceView::GetSerializedDataTag(), _objectGraphFactory );
	}

// ---------------------------------------------------

	void EngineService::OnServiceTickStarted( WorkerContext& /*executingContext*/ ) {
		GetScriptEngine().GarbageCollect( ::asGC_DETECT_GARBAGE | ::asGC_DESTROY_GARBAGE | ::asGC_ONE_STEP );
	}

// ---------------------------------------------------

	StringMarshal EngineService::MarshalStringLiteral( const unsigned int literalLengthInOctets, const UTF8Char* const stringLiteral ) {
		return StringMarshal( stringLiteral, literalLengthInOctets, _allocator );
	}

// ---------------------------------------------------

	void EngineService::MessageCallback( const ::asSMessageInfo* messageInfo ) {
		LogMessageType	messageType;
		const char*		description;
		
		switch( messageInfo->type ) {
			case asMSGTYPE_ERROR: {
				messageType = LogMessageType::Error;
				description = "error";
				break;
			}
			case asMSGTYPE_WARNING: {
				messageType = LogMessageType::Warning;
				description = "warning";
				break;
			}
			case asMSGTYPE_INFORMATION:
			default: {
				messageType = LogMessageType::Message;
				description = "message";
				break;
			}
		}

		GetLogger( messageType )( UTF8L("Angelscript {} in '{}'[{}, {}]: {}.") ET_UTF8_NEWLINE_LITERAL, description, messageInfo->section, messageInfo->row, messageInfo->col, messageInfo->message );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif