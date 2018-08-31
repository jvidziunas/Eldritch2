/*==================================================================*\
  XAudio2AudioScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/Xaudio2Listener.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	XAudio2AudioScene::XAudio2AudioScene() :
		_listeners(MallocAllocator("XAudio2 Scene Listener Set Allocator")),
		_operationSet(0u) {
	}

	// ---------------------------------------------------

	void XAudio2AudioScene::UpdateDsp(float32 timeScale) const {
		const UINT32 updateId(_operationSet++);
		for (XAudio2Listener* listener : _listeners) {
			listener->UpdateVoices(_audioSettings, timeScale, updateId);
		}
	}

}}} // namespace Eldritch2::Audio::XAudio2
