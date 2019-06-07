/*==================================================================*\
  OpusDecoder.hpp
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

namespace Eldritch2 { namespace Audio {
	template <typename Sample>
	class PcmCache;
}} // namespace Eldritch2::Audio
struct OpusDecoder;

namespace Eldritch2 { namespace Audio {

	class OpusStreamDecoder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		OpusStreamDecoder(const OpusStreamDecoder&) = delete;
		//!	Constructs this @ref OpusStreamDecoder instance.
		OpusStreamDecoder(OpusStreamDecoder&&) ETNoexceptHint;
		//!	Constructs this @ref OpusStreamDecoder instance.
		OpusStreamDecoder() ETNoexceptHint;

		~OpusStreamDecoder();

		// ---------------------------------------------------

	public:
		Result DecodeGrain(float32*& outSamples, PcmCache<float32>& cache, uint32 sampleHz, uint32 grain);
		Result DecodeGrain(int16*& outSamples, PcmCache<int16>& cache, uint32 sampleHz, uint32 grain);

		// ---------------------------------------------------

	public:
		Result BindResources(uint32 channels, uint32 maxSampleHz, const uint32 seekTable[], const byte opusBytes[]);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy construction.
		OpusStreamDecoder& operator=(const OpusStreamDecoder&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		OpusDecoder*  _decoder;
		const uint32* _seekTable;
		const byte*   _opusBytes;

		// ---------------------------------------------------

		friend void Swap(OpusStreamDecoder&, OpusStreamDecoder&) ETNoexceptHint;
	};

}} // namespace Eldritch2::Audio
