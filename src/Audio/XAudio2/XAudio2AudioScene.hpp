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
#include <Audio/XAudio2/StreamingVoice.hpp>
#include <Audio/OpusStreamDecoder.hpp>
#include <Audio/PcmCache.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	class XAudio2Listener;
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	class XAudio2AudioScene {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using OpusVoice = StreamingVoice<int16, OpusStreamDecoder>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		XAudio2AudioScene(const XAudio2AudioScene&) = delete;
		//!	Constructs this @ref XAudio2AudioScene instance.
		XAudio2AudioScene() ETNoexceptHint;

		~XAudio2AudioScene() = default;

		// ---------------------------------------------------

	public:
		Span<XAudio2Listener* const*> GetListeners() const ETNoexceptHint;

		Span<OpusVoice*> GetMonoVoices() ETNoexceptHint;

		PcmCache<int16>& GetMonoSamples() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void PlayDiegeticSound(Transformation localToWorld, MicrosecondTime startTime);

		// ---------------------------------------------------

	public:
		void Insert(XAudio2Listener& listener);

		void Erase(XAudio2Listener& listener);

		// ---------------------------------------------------

	public:
		HRESULT BindResources(IXAudio2* xaudio, ArrayList<OpusVoice>::SizeType voices);

		void FreeResources(IXAudio2* xaudio) ETNoexceptHint;

		// ---------------------------------------------------

	private:
		PcmCache<int16>            _samplesByChannelCount[2];
		ArrayList<OpusVoice>       _voicesByChannelCount[2];
		ArraySet<XAudio2Listener*> _listeners;
	};

}}} // namespace Eldritch2::Audio::XAudio2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.inl>
//------------------------------------------------------------------//
