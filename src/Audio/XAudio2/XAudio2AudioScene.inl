/*==================================================================*\
  XAudio2AudioScene.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	ETInlineHint ETForceInlineHint Span<XAudio2Listener* const*> XAudio2AudioScene::GetListeners() const ETNoexceptHint {
		return { _listeners.Begin(), _listeners.End() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Span<XAudio2AudioScene::OpusVoice*> XAudio2AudioScene::GetMonoVoices() ETNoexceptHint {
		return { _voicesByChannelCount[0].Begin(), _voicesByChannelCount[0].End() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint PcmCache<int16>& XAudio2AudioScene::GetMonoSamples() ETNoexceptHint {
		return _samplesByChannelCount[0];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void XAudio2AudioScene::Insert(XAudio2Listener& listener) {
		_listeners.Insert(ETAddressOf(listener));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void XAudio2AudioScene::Erase(XAudio2Listener& listener) {
		_listeners.Erase(ETAddressOf(listener));
	}

}}} // namespace Eldritch2::Audio::XAudio2
