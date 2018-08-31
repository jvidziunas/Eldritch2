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

	ETInlineHint ETForceInlineHint void XAudio2AudioScene::AddListener(XAudio2Listener& listener) {
		_listeners.Insert(ETAddressOf(listener));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void XAudio2AudioScene::RemoveListener(XAudio2Listener& listener) {
		_listeners.Erase(ETAddressOf(listener));
	}

}}} // namespace Eldritch2::Audio::XAudio2
