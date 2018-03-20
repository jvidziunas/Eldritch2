/*==================================================================*\
  XAudio2Listener.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2Listener.hpp>
#include <Audio/XAudio2/XAudioMarshals.hpp>
//------------------------------------------------------------------//
#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#else
#	include <XAudio2.h>
#endif
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY( "XAudio2.lib" )
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {
namespace XAudio2 {

	XAudio2Voice::XAudio2Voice(
		float32 loudnessDb,
		IXAudio2SourceVoice* sourceVoice,
		IXAudio2Voice* effectVoice
	) : Voice( loudnessDb ),
		_sourceVoice( sourceVoice ),
		_effectVoice( effectVoice ) {
	}

// ---------------------------------------------------

	XAudio2Voice::XAudio2Voice(
		XAudio2Voice&& voice
	) : Voice( voice ),
		_sourceVoice( eastl::exchange( voice._sourceVoice, nullptr ) ),
		_effectVoice( eastl::exchange( voice._effectVoice, nullptr ) ) {}

// ---------------------------------------------------

	XAudio2Voice::~XAudio2Voice() {
		if (_effectVoice) {
			_effectVoice->DestroyVoice();
		}
		
		if (_sourceVoice) {
			_sourceVoice->DestroyVoice();
		}
	}

// ---------------------------------------------------

	void XAudio2Voice::UpdateDspSettings( const X3DAUDIO_HANDLE settings, const X3DAUDIO_LISTENER& listener, float32 timeScalar ) const {
		enum : uint32 {
			MaxSourceChannels	= 8u,
			MaxOutputChannels	= MaxSourceChannels,
			CalculateFlags		= X3DAUDIO_CALCULATE_MATRIX
								| X3DAUDIO_CALCULATE_LPF_DIRECT
								| X3DAUDIO_CALCULATE_LPF_REVERB
								| X3DAUDIO_CALCULATE_REVERB
								| X3DAUDIO_CALCULATE_DOPPLER
								| X3DAUDIO_CALCULATE_EMITTER_ANGLE
		};

		X3DAUDIO_DSP_SETTINGS	dspSettings;
		FLOAT32					levelMatrix[MaxOutputChannels * MaxSourceChannels];
		FLOAT32					delayMatrix[MaxOutputChannels];

		dspSettings.SrcChannelCount		= 1u;
		dspSettings.DstChannelCount		= 8u;
		dspSettings.pMatrixCoefficients = levelMatrix;
		dspSettings.pDelayTimes			= delayMatrix;

		X3DAudioCalculate( settings, &listener, &_emitter, CalculateFlags, &dspSettings );

		_sourceVoice->SetOutputMatrix( nullptr, dspSettings.SrcChannelCount, dspSettings.DstChannelCount, dspSettings.pMatrixCoefficients );
//		_sourceVoice->SetVolume( GetHdrAmplitudeScalar() );
		_sourceVoice->SetFrequencyRatio( dspSettings.DopplerFactor * timeScalar );
	}

// ---------------------------------------------------

	XAudio2Listener::XAudio2Listener( Transformation localToWorld ) : Listener<XAudio2Voice>() {
		SetLocalToWorld( localToWorld );

		_listener.Velocity	= X3DAUDIO_VECTOR{ 0.0f, 0.0f, 0.0f };
		_listener.pCone		= nullptr;
	}

// ---------------------------------------------------

	void XAudio2Listener::UpdateVoices( const X3DAUDIO_HANDLE settings, float32 timeScalar ) {
		const float32	loudness( GetCurrentLoudnessDb() );

		for (const XAudio2Voice& voice : GetActiveVoices()) {
			voice.UpdateDspSettings( settings, _listener, timeScalar );
		}
	}

// ---------------------------------------------------

	Transformation ETSimdCall XAudio2Listener::GetLocalToWorld() const {
		return Transformation( AsVector( _listener.Position ), AsBasis( AsVector( _listener.OrientFront ), AsVector( _listener.OrientTop ) ) );
	}

// ---------------------------------------------------

	Transformation ETSimdCall XAudio2Listener::GetWorldToLocal() const {
		return Transformation( -AsVector( _listener.Position ), AsBasis( -AsVector( _listener.OrientFront ), -AsVector( _listener.OrientTop ) )  );
	}

// ---------------------------------------------------

	void ETSimdCall XAudio2Listener::SetLocalToWorld( Transformation value ) {
		_listener.OrientFront	= AsX3dAudioVector( value.rotation.GetForward() );
		_listener.OrientTop		= AsX3dAudioVector( value.rotation.GetUp() );
		_listener.Position		= AsX3dAudioVector( value.translation );
	}

}	// namespace XAudio2
}	// namespace Audio
}	// namespace Eldritch2