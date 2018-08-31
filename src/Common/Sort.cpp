/*==================================================================*\
  Sort.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Sort.hpp>
//------------------------------------------------------------------//
#include <smmintrin.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	enum : int {
		ReverseMask      = _MM_SHUFFLE(0, 1, 2, 3),
		HighWordsMask    = _MM_SHUFFLE(3, 1, 3, 1),
		LowWordsMask     = _MM_SHUFFLE(0, 2, 0, 2),
		UnstripeHiLoMask = _MM_SHUFFLE(0, 2, 1, 3),
		StripeHiLoMask   = _MM_SHUFFLE(3, 1, 2, 0)
	};

	// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint __m128i _mm_reverse_epi32(__m128i x) {
		return _mm_shuffle_epi32(x, ReverseMask);
	}

	// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint __m128i _mm_unstripehi_epi32(__m128i x, __m128i y) {
		//	NOTE: OLDER MODEL INTEL PROCESSORS (pre-Sandy Bridge) INCUR A PENALTY TO LATENCY WHEN SHIFTING FROM FP -> INTEGER SIMD UNIT
		return _mm_shuffle_epi32(_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(x), _mm_castsi128_ps(y), HighWordsMask)), StripeHiLoMask);
	}

	// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint __m128i _mm_unstripelo_epi32(__m128i x, __m128i y) {
		//	NOTE: OLDER MODEL INTEL PROCESSORS (pre-Sandy Bridge) INCUR A PENALTY TO LATENCY WHEN SHIFTING FROM FP -> INTEGER SIMD UNIT
		return _mm_shuffle_epi32(_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(x), _mm_castsi128_ps(y), LowWordsMask)), StripeHiLoMask);
	}

	// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint __m128i _mm_stripehi_epi32(__m128i x, __m128i y) {
		//	NOTE: OLDER MODEL INTEL PROCESSORS (pre-Sandy Bridge) INCUR A PENALTY TO LATENCY WHEN SHIFTING FROM FP -> INTEGER SIMD UNIT
		return _mm_shuffle_epi32(_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(x), _mm_castsi128_ps(y), HighWordsMask)), StripeHiLoMask);
	}

	// ---------------------------------------------------

	static ETInlineHint ETPureFunctionHint __m128i _mm_stripelo_epi32(__m128i x, __m128i y) {
		//	NOTE: OLDER MODEL INTEL PROCESSORS (pre-Sandy Bridge) INCUR A PENALTY TO LATENCY WHEN SHIFTING FROM FP -> INTEGER SIMD UNIT
		return _mm_shuffle_epi32(_mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(x), _mm_castsi128_ps(y), LowWordsMask)), StripeHiLoMask);
	}

} // anonymous namespace

void Sse41MergeSort(__m128i* list0, __m128i* list0End, __m128i* list1, __m128i* out) {
	register __m128i keysA   = _mm_unstripehi_epi32(_mm_load_si128(list0), _mm_load_si128(list0 + 1));
	register __m128i valuesA = _mm_unstripelo_epi32(_mm_load_si128(list0), _mm_load_si128(list0 + 1));

	for (; list0 != list0End; list0 += 2, list1 += 2, out += 2) {
		register __m128i keysB   = _mm_unstripehi_epi32(_mm_load_si128(list1), _mm_load_si128(list1 + 1));
		register __m128i valuesB = _mm_unstripelo_epi32(_mm_load_si128(list1), _mm_load_si128(list1 + 1));

		//	Reverse elements of B.
		keysB   = _mm_reverse_epi32(keysB);
		valuesB = _mm_reverse_epi32(valuesB);

		register __m128i minKeys   = _mm_min_epu32(keysA, keysB);
		register __m128i maxKeys   = _mm_max_epu32(keysA, keysB);
		register __m128i minValues = _mm_blendv_epi8(valuesA, valuesB, _mm_cmpeq_epi32(keysA, minKeys));
		register __m128i maxValues = _mm_blendv_epi8(valuesB, valuesA, _mm_cmpeq_epi32(keysA, minKeys));

		minKeys   = _mm_min_epu32(keysA, keysB);
		maxKeys   = _mm_max_epu32(keysA, keysB);
		minValues = _mm_shuffle_epi32(_mm_blendv_epi8(minValues, maxValues, _mm_cmpeq_epi32(keysA, minKeys)), _MM_SHUFFLE(3, 2, 1, 0));
		maxValues = _mm_shuffle_epi32(_mm_blendv_epi8(maxValues, minValues, _mm_cmpeq_epi32(keysA, minKeys)), _MM_SHUFFLE(3, 2, 1, 0));

		minKeys   = _mm_min_epu32(keysA, keysB);
		maxKeys   = _mm_max_epu32(keysA, keysB);
		minValues = _mm_blendv_epi8(minValues, maxValues, _mm_cmpeq_epi32(keysA, minKeys));
		maxValues = _mm_blendv_epi8(maxValues, minValues, _mm_cmpeq_epi32(keysA, minKeys));

		_mm_stream_si128(out, _mm_stripehi_epi32(keysA, valuesA));
		_mm_stream_si128(out + 1, _mm_stripelo_epi32(keysA, valuesA));
	}
}

} // namespace Eldritch2
