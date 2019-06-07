/*==================================================================*\
  PcmCache.inl
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
	ETInlineHint ETForceInlineHint PcmCache<Sample>::PcmCache() ETNoexceptHint : _samplesPerGrain(0u), _samples(MallocAllocator("PCM Cache Sample Allocator")) {}

	// ---------------------------------------------------

	template <typename Sample>
	ETInlineHint ETForceInlineHint bool PcmCache<Sample>::ShouldDecode(Sample*& /*outSamples*/) {
		return false;
	}

	// ---------------------------------------------------

	template <typename Sample>
	ETInlineHint ETForceInlineHint Result PcmCache<Sample>::BindResources(typename ArrayList<Sample>::SizeType cachedGrains, uint32 sampleRateHz, typename ArrayList<Sample>::SizeType grainLength) {
		using ::Eldritch2::Swap;

		typename ArrayList<Sample>::SizeType samplesPerGrain(sampleRateHz * grainLength / 1000000u);
		ArrayList<Sample>                    samples(_samples.GetAllocator());
		samples.Resize(cachedGrains * samplesPerGrain);

		Swap(_samplesPerGrain, samplesPerGrain);
		Swap(_samples, samples);

		return Result::Success;
	}

	// ---------------------------------------------------

	template <typename Sample>
	ETInlineHint ETForceInlineHint void PcmCache<Sample>::FreeResources() ETNoexceptHint {
		_samples.Clear(ReleaseMemorySemantics());
		_samplesPerGrain = 0u;
	}

	// ---------------------------------------------------

	template <typename Sample2>
	ETInlineHint void Swap(PcmCache<Sample2>& lhs, PcmCache<Sample2>& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._samplesPerGrain, rhs._samplesPerGrain);
		Swap(lhs._samples, rhs._samples);
	}

}} // namespace Eldritch2::Audio
