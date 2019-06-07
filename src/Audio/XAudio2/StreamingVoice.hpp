/*==================================================================*\
  StreamingVoice.hpp
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
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio {
	template <typename Sample>
	class PcmCache;
}} // namespace Eldritch2::Audio

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	template <typename Sample, typename Decoder>
	class StreamingVoice {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		StreamingVoice(const StreamingVoice&) = delete;
		//!	Constructs this @ref StreamingVoice instance.
		StreamingVoice(StreamingVoice&&);
		//!	Constructs this @ref StreamingVoice instance.
		StreamingVoice() ETNoexceptHint;

		~StreamingVoice() = default;

		// ---------------------------------------------------

	public:
		HRESULT Decode(PcmCache<Sample>& target, uint32 mixaheadLimit);

		HRESULT UpdateDsp(UINT32 operationSet, float32 volume, float32 doppler, const float32 levelMatrix[]);

		// ---------------------------------------------------

	public:
		template <typename... DecoderArgs>
		HRESULT Acquire(UINT32 operationSet, IXAudio2Voice* client, DecoderArgs&&... decoderArgs) ETNoexceptHint;

		void Release() ETNoexceptHint;

		// ---------------------------------------------------

	public:
		HRESULT BindResources(IXAudio2* xaudio, DWORD channelMask, DWORD sampleHz);

		void FreeResources(IXAudio2* xaudio) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<IXAudio2Voice*> _client;
		IXAudio2SourceVoice*   _voice;
		uint32                 _channels;
		DWORD                  _sampleHz;
		uint32                 _position;
		Decoder                _decoder;
	};

}}} // namespace Eldritch2::Audio::XAudio2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/XAudio2/StreamingVoice.inl>
//------------------------------------------------------------------//
