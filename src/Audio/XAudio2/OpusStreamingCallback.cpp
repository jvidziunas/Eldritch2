/*==================================================================*\
  OpusStreamingCallback.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/OpusStreamingCallback.hpp>
//------------------------------------------------------------------//
#include <opus.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("opus.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	namespace {

		enum : int {
			DecodeForwardError = 1,
			DropForwardError   = 0
		};

	} // anonymous namespace

	// opus_decode( OpusDecoder *st, const unsigned char *data, opus_int32 len, opus_int16 *pcm, int frame_size, int decode_fec );

	OpusStreamingCallback::OpusStreamingCallback(int channelCount) :
		_producedBuffers(0u),
		_consumedBuffers(0u) {
		ET_ASSERT(channelCount > 0, "Opus stream must have at least one channel!");

		opus_decoder_init(reinterpret_cast<OpusDecoder*>(_decoderMemory), OutputSampleRateInHz, channelCount);
	}

	// ---------------------------------------------------

	OpusStreamingCallback::~OpusStreamingCallback() {
		opus_decoder_destroy(reinterpret_cast<OpusDecoder*>(_decoderMemory));
	}

	// ---------------------------------------------------

	void OpusStreamingCallback::OnBufferStart(void* /*completedBuffer*/) {
		_consumedBuffers.fetch_add(1u, std::memory_order_relaxed);
	}

	// ---------------------------------------------------

	void OpusStreamingCallback::OnBufferEnd(void* /*completedBuffer*/) {}

	// ---------------------------------------------------

	void OpusStreamingCallback::OnLoopEnd(void* /*completedBuffer*/) {}

}}} // namespace Eldritch2::Audio::XAudio2
