/*==================================================================*\
  XAudio2Listener.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/Xaudio2Selector.hpp>
#include <Audio/Listener.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	class OpusStreamDecoder;
	template <typename Sample, typename Decoder>
	class StreamingVoice;
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	class XAudio2DiegeticVoice : public DiegeticVoice {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using OpusVoice = StreamingVoice<int16, OpusStreamDecoder>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref XAudio2DiegeticVoice instance.
		XAudio2DiegeticVoice(Transformation localToWorld, MicrosecondTime startTime, float32 loudnessDb);
		//!	Disable copy construction.
		XAudio2DiegeticVoice(const XAudio2DiegeticVoice&) = delete;
		//!	Constructs this @ref XAudio2Voice instance.
		XAudio2DiegeticVoice(XAudio2DiegeticVoice&&);

		~XAudio2DiegeticVoice();

		// ---------------------------------------------------

	public:
		HRESULT MakeActive(IXAudio2* xaudio, IXAudio2SubmixVoice* diegeticMix, OpusVoice& source);

		void MakeVirtual();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		XAudio2DiegeticVoice& operator=(const XAudio2DiegeticVoice&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		OpusVoice*           _source;
		IXAudio2SubmixVoice* _effect;

		// ---------------------------------------------------

		friend void Swap(XAudio2DiegeticVoice&, XAudio2DiegeticVoice&);
	};

	// ---

	class XAudio2Listener : public Listener<XAudio2DiegeticVoice> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref XAudio2Listener instance.
		XAudio2Listener(Transformation localToWorld, Vector linearVelocity) ETNoexceptHint;
		//!	Disable copy construction.
		XAudio2Listener(const XAudio2Listener&) = delete;

		~XAudio2Listener() = default;

		// ---------------------------------------------------

	public:
		void Mix(UINT32 operationSet, float32 timeScalar) const;

		// ---------------------------------------------------

	public:
		HRESULT BindResources(IXAudio2* xaudio, uint32 masteringHz, DWORD channelMask);

		void FreeResources();

		// - DATA MEMBERS ------------------------------------

	private:
		X3DAUDIO_HANDLE      _settings;
		IXAudio2SubmixVoice* _diegeticMix;
		IXAudio2SubmixVoice* _nonDiegeticMix;
	};

}}} // namespace Eldritch2::Audio::XAudio2
