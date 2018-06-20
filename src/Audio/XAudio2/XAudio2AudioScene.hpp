/*==================================================================*\
  Xaudio2AudioScene.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Audio {
		namespace XAudio2 {
			class	XAudio2Listener;
		}
	}
}

namespace Eldritch2 {
	namespace Audio {
		namespace XAudio2 {

			class XAudio2AudioScene {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
			//!	Disable copy construction.
				XAudio2AudioScene(const XAudio2AudioScene&) = delete;
			//!	Constructs this @ref XAudio2AudioScene instance.
				XAudio2AudioScene();

				~XAudio2AudioScene() = default;

			// ---------------------------------------------------

			public:
				void	AddListener(XAudio2Listener& listener);

				void	RemoveListener(XAudio2Listener& listener);

			// ---------------------------------------------------

			private:
				ArrayList<XAudio2Listener*>	_listeners;
			};

		}	// namespace XAudio2
	}	// namespace Audio
}	// namespace Eldritch2