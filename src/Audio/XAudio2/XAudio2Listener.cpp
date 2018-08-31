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
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#else
#	include <XAudio2.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	XAudio2Voice::XAudio2Voice(float32 loudnessDb, IXAudio2SourceVoice* sourceVoice, IXAudio2Voice* effectVoice) :
		Voice(loudnessDb),
		_sourceVoice(sourceVoice),
		_effectVoice(effectVoice) {
	}

	// ---------------------------------------------------

	XAudio2Voice::XAudio2Voice(XAudio2Voice&& voice) :
		Voice(voice),
		_sourceVoice(eastl::exchange(voice._sourceVoice, nullptr)),
		_effectVoice(eastl::exchange(voice._effectVoice, nullptr)) {
	}

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

	void XAudio2Voice::UpdateDsp(const XAudio2Listener& listener, const X3DAUDIO_HANDLE& settings, float32 timeScalar, UINT32 operationSet) const {
		enum : uint32 {
			MaxSourceChannels = 4u,
			MaxOutputChannels = 8u,
			CalculateFlags    = X3DAUDIO_CALCULATE_MATRIX
				| X3DAUDIO_CALCULATE_LPF_DIRECT
				| X3DAUDIO_CALCULATE_LPF_REVERB
				| X3DAUDIO_CALCULATE_REVERB
				| X3DAUDIO_CALCULATE_DOPPLER
				| X3DAUDIO_CALCULATE_EMITTER_ANGLE
		};

		X3DAUDIO_DSP_SETTINGS dsp {
			/*pMatrixCoefficients =*/ETStackAlloc(FLOAT32, MaxOutputChannels * MaxSourceChannels),
			/*pDelayTimes =*/ETStackAlloc(FLOAT32, MaxOutputChannels),
			/*SrcChannelCount =*/1u,
			/*DstChannelCount =*/2u
		};

		X3DAudioCalculate(settings, &listener._listener, &_emitter, CalculateFlags, &dsp);
		_sourceVoice->SetOutputMatrix(_effectVoice, dsp.SrcChannelCount, dsp.DstChannelCount, dsp.pMatrixCoefficients, operationSet);
		_sourceVoice->SetVolume(GetHdrAmplitudeScalar(listener.GetWindowTop()), operationSet);
		_sourceVoice->SetFrequencyRatio(dsp.DopplerFactor * timeScalar, operationSet);
	}

	// ---------------------------------------------------

	XAudio2Listener::XAudio2Listener(Transformation localToWorld) :
		Listener<XAudio2Voice>() {
		SetLocalToWorld(localToWorld);
		_listener.Velocity = X3DAUDIO_VECTOR { 0.0f, 0.0f, 0.0f };
		_listener.pCone    = nullptr;
	}

	// ---------------------------------------------------

	void XAudio2Listener::UpdateVoices(const X3DAUDIO_HANDLE& settings, float32 timeScalar, UINT32 operationSet) {
		for (const XAudio2Voice& voice : GetActiveVoices()) {
			voice.UpdateDsp(*this, settings, timeScalar, operationSet);
		}
	}

	// ---------------------------------------------------

	Transformation ETSimdCall XAudio2Listener::GetLocalToWorld() const {
		return Transformation(AsVector(_listener.Position), AsBasis(AsVector(_listener.OrientFront), AsVector(_listener.OrientTop)));
	}

	// ---------------------------------------------------

	Transformation ETSimdCall XAudio2Listener::GetWorldToLocal() const {
		return Transformation(-AsVector(_listener.Position), AsBasis(-AsVector(_listener.OrientFront), -AsVector(_listener.OrientTop)));
	}

	// ---------------------------------------------------

	void ETSimdCall XAudio2Listener::SetLocalToWorld(Transformation value) {
		_listener.OrientFront = AsX3dAudioVector(value.rotation.GetForward());
		_listener.OrientTop   = AsX3dAudioVector(value.rotation.GetUp());
		_listener.Position    = AsX3dAudioVector(value.translation);
	}

}}} // namespace Eldritch2::Audio::XAudio2
