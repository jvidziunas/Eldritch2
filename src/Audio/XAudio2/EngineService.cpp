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
#include <Configuration/ConfigurationRegistrar.hpp>
#include <Audio/XAudio2/EngineService.hpp>
#include <Audio/XAudio2/WorldService.hpp>
#include <Utility/Result.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	ET_LINK_LIBRARY( "XAudio2.lib" )
#else
	ET_LINK_LIBRARY( "ole32.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Core;

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {
namespace {

	MICROPROFILE_DEFINE( audioProcessingPass, "XAudio2 Audio Renderer", "Audio processing pass", 0xCCBBCB );

}	// anonymous namespace
	
	EngineService::EngineService( const Engine& engine ) : Core::EngineService( engine.GetServiceBlackboard() ),
														   _allocator( engine.GetAllocator(), "XAudio2 Audio Renderer Allocator" ),
														   _log( engine.GetLog() ),
														   _forcedSpeakerCount( 0u ),
														   _processorAffinityMask( XAUDIO2_PROCESSOR::XAUDIO2_DEFAULT_PROCESSOR ),
														   _deviceName( { _allocator, "XAudio2 Device Name Allocator" } ),
														   _audioGlitchCount( 0u ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {
		if( !_audio ) {
			return;
		}

		_audio->StopEngine();
	}

// ---------------------------------------------------

	Utf8Literal EngineService::GetName() const {
		return "XAudio2 Audio Renderer";
	}

// ---------------------------------------------------

	Result<UniquePointer<Core::WorldService>> EngineService::CreateWorldService( Allocator& allocator, const World& world ) {
		auto result( MakeUnique<WorldService>( allocator, world ) );

		if( !result ) {
			return Error::OutOfMemory;
		}

		return eastl::move( result );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ConfigurationBroadcastVisitor ) {
		enum : UINT32 {
			XAudio2CreateFlags = (ETIsDebugModeEnabled() ? XAUDIO2_DEBUG_ENGINE : 0)
		};

		ComPointer<IXAudio2>	audio;

		_log( MessageSeverity::Message, "Creating XAudio2 instance." ET_UTF8_NEWLINE_LITERAL );

		if( FAILED( XAudio2Create( audio.GetInterfacePointer(), XAudio2CreateFlags, XAUDIO2_DEFAULT_PROCESSOR ) ) ) {
			_log( MessageSeverity::Error, "Unable to create XAudio2 instance!" ET_UTF8_NEWLINE_LITERAL );

			return;
		}

		if( FAILED( audio->RegisterForCallbacks( this ) ) ) {
			_log( MessageSeverity::Error, "Unable to register XAudio2 device callbacks!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		if( FAILED( audio->StartEngine() ) ) {
			_log( MessageSeverity::Error, "Unable to start XAudio2 engine!" ET_UTF8_NEWLINE_LITERAL );
			return;
		}

		_log( MessageSeverity::Message, "Created XAudio2 instance." ET_UTF8_NEWLINE_LITERAL );

	//	Commit changes to the service.
		_audio = eastl::move( audio );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ConfigurationRegistrar& registrar ) {
		registrar.BeginSection( "XAudio2" )
			.Register( "ForcedSpeakerCount", _forcedSpeakerCount )
			.Register( "AudioProcessingThreadAffinityMask", _processorAffinityMask )
			.Register( "PreferredDeviceName", _deviceName );
	}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( ApiRegistrar& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void EngineService::AcceptVisitor( JobFiber& /*fiber*/, const ServiceTickVisitor ) {
		MICROPROFILE_SCOPEI( GetName(), "Update", 0xABBBCB );

		XAUDIO2_PERFORMANCE_DATA	performanceData;

		_audio->GetPerformanceData( &performanceData );

		if( _audioGlitchCount != performanceData.GlitchesSinceEngineStarted ) {
			_audioGlitchCount = performanceData.GlitchesSinceEngineStarted;

			_log( MessageSeverity::Error, "XAudio reported audio processing stall since last invocation of IXAudio2::GetPerformanceData()!" ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	void EngineService::OnProcessingPassStart() {
	//	MICROPROFILE_SCOPE family of macros assumes you enter and leave the profile scope in the same C++ block, which is not true here.
		MICROPROFILE_CONDITIONAL( _audioProcessingPassTick = MicroProfileEnter( g_mp_audioProcessingPass ) );
	}

// ---------------------------------------------------

	void EngineService::OnProcessingPassEnd() {
	//	MICROPROFILE_SCOPE family of macros assumes you enter and leave the profile scope in the same C++ block, which is not true here.
		MICROPROFILE_CONDITIONAL( MicroProfileLeave( g_mp_audioProcessingPass, _audioProcessingPassTick ) );
	}

// ---------------------------------------------------

	void EngineService::OnCriticalError( HRESULT /*error*/ ) {
	//	TODO: Convert the HRESULT value into something printable.
		_log( MessageSeverity::Error, "Critical error in XAudio!" );
	}

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2