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
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Sound/XAudio2/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Foundation/GameEngine.hpp>
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
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Sound;
using namespace ::Eldritch2;

#ifdef ERROR
#	undef ERROR	
#endif

namespace {

	MICROPROFILE_DEFINE( audioProcessingPass, "XAudio2 Audio Renderer", "Audio processing pass", 0xCCBBCB );

}	// anonymous namespace

namespace Eldritch2 {
namespace Sound {
namespace XAudio2 {
	
	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
															   _allocator( GetEngineAllocator(), UTF8L("XAudio2 Audio Renderer Allocator") ),
															   _forcedSpeakerCount( 0u ),
															   _processorAffinityMask( ::XAUDIO2_DEFAULT_PROCESSOR ),
															   _deviceName( GetEngineAllocator() ),
															   _audioGlitchCount( 0u ) {}

// ---------------------------------------------------

	EngineService::~EngineService() {
		if( _audio ) {
			_audio->StopEngine();
		}
	}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("XAudio2 Audio Renderer");
	}

// ---------------------------------------------------

	void EngineService::OnEngineConfigurationBroadcast( WorkerContext& /*executingContext*/ ) {
		decltype(_audio)	audio;

		GetLogger()( UTF8L("Creating XAudio2 instance.") ET_UTF8_NEWLINE_LITERAL );

		if( FAILED( ::XAudio2Create( audio.GetInterfacePointer(), (ETIsDebugModeEnabled() ? XAUDIO2_DEBUG_ENGINE : 0), ::XAUDIO2_DEFAULT_PROCESSOR ) ) || FAILED( audio->RegisterForCallbacks( this ) ) ) {
			GetLogger( LogMessageType::Error )( UTF8L("Unable to create XAudio2 instance!") ET_UTF8_NEWLINE_LITERAL );

			return;
		}

		if( FAILED( audio->StartEngine() ) ) {
			GetLogger( LogMessageType::Error )( UTF8L("Unable to start XAudio2 engine!") ET_UTF8_NEWLINE_LITERAL );
		}

		GetLogger()( UTF8L("Created XAudio2 instance.") ET_UTF8_NEWLINE_LITERAL );

		_audio = ::std::move( audio );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("XAudio2") );

		visitor.Register( UTF8L("ForcedSpeakerCount"), _forcedSpeakerCount ).Register( UTF8L("AudioProcessingThreadAffinityMask"), _processorAffinityMask );
		visitor.Register( UTF8L("PreferredDeviceName"), _deviceName );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void EngineService::OnServiceTickStarted( WorkerContext& /*executingContext*/ ) {
		MICROPROFILE_SCOPEI( "XAudio2 Audio Renderer", "Update", 0xABBBCB );

		::XAUDIO2_PERFORMANCE_DATA	performanceData;

		_audio->GetPerformanceData( &performanceData );

		if( _audioGlitchCount != performanceData.GlitchesSinceEngineStarted ) {
			_audioGlitchCount = performanceData.GlitchesSinceEngineStarted;

			GetLogger( LogMessageType::Error )( UTF8L("XAudio reported audio processing stall since last invocation of IXAudio2::GetPerformanceData()!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	void EngineService::OnProcessingPassStart() {
		MICROPROFILE_CONDITIONAL( _audioProcessingPassTick = ::MicroProfileEnter( g_mp_audioProcessingPass ) );
	}

// ---------------------------------------------------

	void EngineService::OnProcessingPassEnd() {
		MICROPROFILE_CONDITIONAL( ::MicroProfileLeave( g_mp_audioProcessingPass, _audioProcessingPassTick ) );
	}

// ---------------------------------------------------

	void EngineService::OnCriticalError( ::HRESULT /*error*/ ) {
		GetLogger( LogMessageType::Error )( UTF8L("Critical error in XAudio!") );
	}

}	// namespace XAudio2
}	// namespace Sound
}	// namespace Eldritch2