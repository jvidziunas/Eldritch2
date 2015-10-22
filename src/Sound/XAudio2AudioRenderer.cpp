/*==================================================================*\
  XAudio2AudioRenderer.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scheduler/CRTPTransientTask.hpp>
#include <Sound/XAudio2AudioRenderer.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Foundation/Application.hpp>
#include <Foundation/GameEngine.hpp>
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
using namespace ::std;

#ifdef ERROR
#	undef ERROR	
#endif

namespace Eldritch2 {
namespace Sound {
	
	XAudio2AudioRenderer::XAudio2AudioRenderer( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
																			 _allocator( GetEngineAllocator(), UTF8L("XAudio2 Audio Renderer Allocator") ),
																			 _forcedSpeakerCount( 0u ),
																			 _processorAffinityMask( ::XAUDIO2_DEFAULT_PROCESSOR ),
																			 _deviceName( "", GetEngineAllocator() ),
																			 _audioGlitchCount( 0u ) {}

// ---------------------------------------------------

	XAudio2AudioRenderer::~XAudio2AudioRenderer() {
		if( _audio ) {
			_audio->StopEngine();
		}
	}

// ---------------------------------------------------

	const UTF8Char* const XAudio2AudioRenderer::GetName() const {
		return UTF8L("XAudio2 Audio Renderer");
	}

// ---------------------------------------------------

	void XAudio2AudioRenderer::AcceptTaskVisitor( Allocator& subtaskAllocator, Task& visitingTask, WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) {
		class InitalizeXAudioTask : public CRTPTransientTask<InitalizeXAudioTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this InitializeXAudioTask instance.
			ETInlineHint InitalizeXAudioTask( XAudio2AudioRenderer& host, Task& visitingTask, WorkerContext& executingContext ) : CRTPTransientTask<InitalizeXAudioTask>( visitingTask, Scheduler::CodependentTaskSemantics ),
																																  _host( host ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Initialize XAudio2 Task");
			}

		// ---------------------------------------------------

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				_host.InitializeXAudio();
				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			XAudio2AudioRenderer&	_host;
		};

	// ---

		new(subtaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) InitalizeXAudioTask( *this, visitingTask, executingContext );
	}

// ---------------------------------------------------

	void XAudio2AudioRenderer::AcceptTaskVisitor( Allocator& /*subtaskAllocator*/, Task& /*visitingTask*/, WorkerContext& /*executingContext*/, const ServiceTickTaskVisitor ) {
		::XAUDIO2_PERFORMANCE_DATA	performanceData;

		_audio->GetPerformanceData( &performanceData );

		if( _audioGlitchCount != performanceData.GlitchesSinceEngineStarted ) {
			_audioGlitchCount = performanceData.GlitchesSinceEngineStarted;

			GetLogger( LogMessageType::ERROR )( UTF8L("XAudio reported audio processing stall since last invocation of IXAudio2::GetPerformanceData()!") ET_UTF8_NEWLINE_LITERAL );
		}
	}

// ---------------------------------------------------

	void XAudio2AudioRenderer::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("XAudio2") );

		visitor.Register( UTF8L("ForcedSpeakerCount"), _forcedSpeakerCount ).Register( UTF8L("AudioProcessingThreadAffinityMask"), _processorAffinityMask );
		visitor.Register( UTF8L("PreferredDeviceName"), _deviceName );
	}

// ---------------------------------------------------

	void XAudio2AudioRenderer::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	ErrorCode XAudio2AudioRenderer::InitializeXAudio() {
		decltype(_audio)	audio;

		GetLogger()( UTF8L("Creating XAudio2 instance.") ET_UTF8_NEWLINE_LITERAL );

		if( FAILED( ::XAudio2Create( audio.GetInterfacePointer(), (ETIsDebugModeEnabled() ? XAUDIO2_DEBUG_ENGINE : 0), ::XAUDIO2_DEFAULT_PROCESSOR ) ) || FAILED( audio->StartEngine() ) ) {
			GetLogger( LogMessageType::ERROR )( UTF8L("Unable to create XAudio2 instance!") ET_UTF8_NEWLINE_LITERAL );

			return Error::UNSPECIFIED;
		}

		GetLogger()( UTF8L("Created XAudio2 instance.") ET_UTF8_NEWLINE_LITERAL );

		_audio = move( audio );

		return Error::NONE;
	}

// ---------------------------------------------------

	void XAudio2AudioRenderer::OnProcessingPassStart() {}

// ---------------------------------------------------

	void XAudio2AudioRenderer::OnProcessingPassEnd() {}

// ---------------------------------------------------

	void XAudio2AudioRenderer::OnCriticalError( ::HRESULT /*error*/ ) {
		GetLogger( LogMessageType::ERROR )( UTF8L("Critical error in XAudio!") );
	}

}	// namespace Sound
}	// namespace Eldritch2