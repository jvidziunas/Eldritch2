/*==================================================================*\
  XAudio2AudioScene.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#if ET_PLATFORM_WINDOWS
#	include <windows.h>
#endif
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/X3DAudio.h>
#else
#	include <X3DAudio.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	class XAudio2Listener;
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

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
		void UpdateDsp(float32 timeScale) const;

		// ---------------------------------------------------

	public:
		void AddListener(XAudio2Listener& listener);

		void RemoveListener(XAudio2Listener& listener);

		// ---------------------------------------------------

	private:
		ArraySet<XAudio2Listener*> _listeners;
		X3DAUDIO_HANDLE            _audioSettings;
		mutable UINT32             _operationSet;
	};

}}} // namespace Eldritch2::Audio::XAudio2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.inl>
//------------------------------------------------------------------//
