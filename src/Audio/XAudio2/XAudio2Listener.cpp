/*==================================================================*\
  XAudio2Listener.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2Listener.hpp>
#include <Audio/XAudio2/XAudioMarshals.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	namespace {

		ETForceInlineHint ETPureFunctionHint bool Failed(HRESULT result) ETNoexceptHint {
			return FAILED(result);
		}

	} // anonymous namespace

	XAudio2DiegeticVoice::XAudio2DiegeticVoice(Transformation localToWorld, MicrosecondTime startTime, float32 loudnessDb) :
		DiegeticVoice(localToWorld, startTime, loudnessDb),
		_source(nullptr),
		_effect(nullptr) {}

	// ---------------------------------------------------

	XAudio2DiegeticVoice::XAudio2DiegeticVoice(XAudio2DiegeticVoice&& voice) :
		DiegeticVoice(Move(voice)),
		_source(Exchange(_source, nullptr)),
		_effect(Exchange(voice._effect, nullptr)) {
	}

	// ---------------------------------------------------

	XAudio2DiegeticVoice::~XAudio2DiegeticVoice() {}

	// ---------------------------------------------------

	HRESULT XAudio2DiegeticVoice::MakeActive(IXAudio2* xaudio, IXAudio2SubmixVoice* diegeticMix, OpusVoice& source) {
		using ::Eldritch2::Swap;

		XAUDIO2_SEND_DESCRIPTOR   sendDescriptors[] = { { /*Flags =*/0u, diegeticMix } };
		const XAUDIO2_VOICE_SENDS sends{ UINT32(ETCountOf(sendDescriptors)), sendDescriptors };

		IXAudio2SubmixVoice* effect(nullptr);
		ET_ABORT_UNLESS(xaudio->CreateSubmixVoice(ETAddressOf(effect), /*InputChannels =*/2u, /*InputSampleRate =*/48000, /*Flags =*/0u, /*ProcessingStage =*/0u, ETAddressOf(sends)));
		ET_AT_SCOPE_EXIT(if (effect) effect->DestroyVoice());

		Swap(_effect, effect);
		return S_OK;
	}

	// ---------------------------------------------------

	void XAudio2DiegeticVoice::MakeVirtual() {
		if (IXAudio2SubmixVoice* const effectVoice = Exchange(_effect, nullptr)) {
			effectVoice->DestroyVoice();
		}

		_source = nullptr;
	}

	// ---------------------------------------------------

	XAudio2Listener::XAudio2Listener(Transformation localToWorld, Vector linearVelocity) ETNoexceptHint : Listener<XAudio2DiegeticVoice>(localToWorld, linearVelocity),
																										  _diegeticMix(nullptr),
																										  _nonDiegeticMix(nullptr) {}

	// ---------------------------------------------------

	void XAudio2Listener::Mix(UINT32 operationSet, float32 timeScalar) const {
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

		X3DAUDIO_DSP_SETTINGS dsp{
			/*pMatrixCoefficients =*/ETStackAlloc(FLOAT32, MaxOutputChannels * MaxSourceChannels),
			/*pDelayTimes =*/ETStackAlloc(FLOAT32, MaxOutputChannels),
			/*SrcChannelCount =*/1u,
			/*DstChannelCount =*/2u
		};

		Transformation          localToWorld(GetLocalToWorld());
		const X3DAUDIO_LISTENER listener{
			/*OrientFront =*/AsX3dAudioVector(localToWorld.rotation.GetForward()),
			/*OrientTop =*/AsX3dAudioVector(localToWorld.rotation.GetUp()),
			/*Position =*/AsX3dAudioVector(localToWorld.translation),
			/*Velocity =*/AsX3dAudioVector(GetLinearVelocity()),
			/*pCone =*/nullptr
		};

		const float32 hdrWindowTopDb(GetWindowTopDb());
		for (ArrayList<XAudio2DiegeticVoice>::ConstReference voice : GetActiveVoices()) {
			const X3DAUDIO_EMITTER emitter{};

			X3DAudioCalculate(_settings, ETAddressOf(listener), ETAddressOf(emitter), CalculateFlags, ETAddressOf(dsp));
		}
	}

	// ---------------------------------------------------

	HRESULT XAudio2Listener::BindResources(IXAudio2* xaudio, uint32 masteringHz, DWORD channelMask) {
		using ::Eldritch2::Swap;

		static ETConstexpr UINT32 ChannelCount(2u);

		X3DAUDIO_HANDLE settings;
		X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, settings);

		IXAudio2SubmixVoice* diegeticMix(nullptr);
		ET_ABORT_UNLESS(xaudio->CreateSubmixVoice(ETAddressOf(diegeticMix), ChannelCount, masteringHz));
		ET_AT_SCOPE_EXIT(if (diegeticMix) diegeticMix->DestroyVoice());

		IXAudio2SubmixVoice* nonDiegeticMix(nullptr);
		ET_ABORT_UNLESS(xaudio->CreateSubmixVoice(ETAddressOf(nonDiegeticMix), ChannelCount, masteringHz));
		ET_AT_SCOPE_EXIT(if (nonDiegeticMix) nonDiegeticMix->DestroyVoice());

		Swap(_settings, settings);
		Swap(_diegeticMix, diegeticMix);
		Swap(_nonDiegeticMix, nonDiegeticMix);

		return S_OK;
	}

	// ---------------------------------------------------

	void XAudio2Listener::FreeResources() {
		if (IXAudio2SubmixVoice* const nonDiegeticMix = Exchange(_nonDiegeticMix, nullptr)) {
			nonDiegeticMix->DestroyVoice();
		}

		if (IXAudio2SubmixVoice* const diegeticMix = Exchange(_diegeticMix, nullptr)) {
			diegeticMix->DestroyVoice();
		}
	}

}}} // namespace Eldritch2::Audio::XAudio2
