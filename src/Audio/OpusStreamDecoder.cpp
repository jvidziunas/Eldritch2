/*==================================================================*\
  OpusStreamDecoder.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/OpusStreamDecoder.hpp>
#include <Audio/PcmCache.hpp>
//------------------------------------------------------------------//
#include <opus.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_LIBRARY("opus.lib")
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio {

	namespace {

		ETConstexpr ETForceInlineHint ETPureFunctionHint Result AsResult(int result) ETNoexceptHint {
			switch (result) {
			case OPUS_OK: return Result::Success;
			case OPUS_BAD_ARG: return Result::InvalidParameter;
			case OPUS_BUFFER_TOO_SMALL: return Result::OutOfMemory;
			case OPUS_INTERNAL_ERROR: return Result::Unspecified;
			case OPUS_INVALID_PACKET: return Result::Unspecified;
			case OPUS_UNIMPLEMENTED: return Result::Unspecified;
			case OPUS_INVALID_STATE: return Result::InvalidObjectState;
			case OPUS_ALLOC_FAIL: return Result::OutOfMemory;
			default: return Result::Unspecified;
			};
		}

	} // anonymous namespace

	OpusStreamDecoder::OpusStreamDecoder() ETNoexceptHint : _decoder(nullptr), _seekTable(nullptr), _opusBytes(nullptr) {}

	// ---------------------------------------------------

	OpusStreamDecoder::OpusStreamDecoder(OpusStreamDecoder&& decoder) ETNoexceptHint : OpusStreamDecoder() {
		Swap(*this, decoder);
	}

	// ---------------------------------------------------

	OpusStreamDecoder::~OpusStreamDecoder() {
		ETAssert(_decoder == nullptr, "Leaking Opus decoder {}!", fmt::ptr(_decoder));
	}

	// ---------------------------------------------------

	Result OpusStreamDecoder::DecodeGrain(float32*& outSamples, PcmCache<float32>& cache, uint32 sampleHz, uint32 grain) {
		const uint32 firstSample(_seekTable[grain]);
		if (!cache.ShouldDecode(outSamples)) {
			return Result::Success;
		}

		return AsResult(opus_decode_float(_decoder, _opusBytes + firstSample, _seekTable[grain + 1] - firstSample, outSamples, /*frame_size =*/sampleHz, /*decode_fec =*/0));
	}

	// ---------------------------------------------------

	Result OpusStreamDecoder::DecodeGrain(int16*& outSamples, PcmCache<int16>& cache, uint32 sampleHz, uint32 grain) {
		const uint32 firstSample(_seekTable[grain]);
		if (!cache.ShouldDecode(outSamples)) {
			return Result::Success;
		}

		
		return AsResult(opus_decode(_decoder, _opusBytes + firstSample, _seekTable[grain + 1] - firstSample, outSamples, /*frame_size =*/sampleHz, /*decode_fec =*/0));
	}

	// ---------------------------------------------------

	Result OpusStreamDecoder::BindResources(uint32 channels, uint32 maxSampleHz, const uint32 seekTable[], const byte opusBytes[]) {
		using ::Eldritch2::Swap;

		OpusDecoder* decoder(nullptr);
		ET_ABORT_UNLESS(AsResult(opus_decoder_init(decoder, opus_int32(maxSampleHz), int(channels))));

		Swap(_decoder, decoder);
		Swap(_seekTable, seekTable);
		Swap(_opusBytes, opusBytes);

		return Result::Success;
	}

	// ---------------------------------------------------

	void OpusStreamDecoder::FreeResources() ETNoexceptHint {
		_opusBytes = nullptr;
		_seekTable = nullptr;
		//	Don't need to call `opus_decoder_destroy()` since we allocated our own memory.
		_decoder = nullptr;
	}

	// ---------------------------------------------------

	void Swap(OpusStreamDecoder& lhs, OpusStreamDecoder& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._decoder, rhs._decoder);
		Swap(lhs._seekTable, rhs._seekTable);
		Swap(lhs._opusBytes, rhs._opusBytes);
	}

}} // namespace Eldritch2::Audio
