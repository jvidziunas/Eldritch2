/*==================================================================*\
  XAudio2AudioScene.cpp
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
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/XAudio2Listener.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	namespace {

		ETForceInlineHint ETPureFunctionHint bool Failed(HRESULT result) ETNoexceptHint {
			return FAILED(result);
		}

	} // anonymous namespace

	XAudio2AudioScene::XAudio2AudioScene() ETNoexceptHint : _samplesByChannelCount{ {} },
															_voicesByChannelCount{ MallocAllocator("XAudio2 Scene Opus Voice List Allocator") },
															_listeners(MallocAllocator("XAudio2 Scene Listener Set Allocator")) {}

	// ---------------------------------------------------

	void XAudio2AudioScene::PlayDiegeticSound(Transformation localToWorld, MicrosecondTime /*startTime*/) {
		for (XAudio2Listener* listener : _listeners) {
			const Vector soundToListener(localToWorld.translation - listener->GetLocalToWorld().translation);
			if (SquaredLength(soundToListener) > 100.0f) {
				continue;
			}

			// listener->PlayDiegeticSound(localToWorld);
		}
	}

	// ---------------------------------------------------

	HRESULT XAudio2AudioScene::BindResources(IXAudio2* xaudio, ArrayList<OpusVoice>::SizeType voiceCount) {
		using ::Eldritch2::Swap;
		static ETConstexpr uint64 GrainLength((1000000u * XAUDIO2_QUANTUM_NUMERATOR) / XAUDIO2_QUANTUM_DENOMINATOR);

		PcmCache<int16> samplesByChannelCount[ETCountOf(_samplesByChannelCount)];
		for (PcmCache<int16>& samples : samplesByChannelCount) {
			samples.BindResources((voiceCount + 16u) * 2u, /*sampleHz =*/48000, GrainLength);
		}

		ArrayList<OpusVoice> voicesByChannelCount[ETCountOf(_voicesByChannelCount)];
		ET_FOREACH_AT_SCOPE_EXIT(voiceList, voicesByChannelCount, voiceList.ClearAndDispose([xaudio](OpusVoice& voice) ETNoexceptHint { voice.FreeResources(xaudio); }));
		for (ArrayList<OpusVoice>& voices : voicesByChannelCount) {
			voices.SetCapacity(voiceCount);
			for (size_t id(0u); id < voiceCount; ++id) {
				ET_ABORT_UNLESS(voices.EmplaceBack().BindResources(xaudio, /*channelMask =*/1u, /*sampleHz =*/48000));
			}
		}

		Swap(_samplesByChannelCount, samplesByChannelCount);
		Swap(_voicesByChannelCount, voicesByChannelCount);

		return S_OK;
	}

	// ---------------------------------------------------

	void XAudio2AudioScene::FreeResources(IXAudio2* xaudio) ETNoexceptHint {
		_listeners.Clear();
		for (ArrayList<OpusVoice>& voiceList : _voicesByChannelCount) {
			voiceList.ClearAndDispose([xaudio](OpusVoice& voice) ETNoexceptHint { voice.FreeResources(xaudio); });
		}
		for (PcmCache<int16>& samples : _samplesByChannelCount) {
			samples.FreeResources();
		}
	}

}}} // namespace Eldritch2::Audio::XAudio2
