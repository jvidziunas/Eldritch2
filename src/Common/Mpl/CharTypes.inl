/*==================================================================*\
  CharTypes.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <EASTL/internal/char_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

namespace Detail {

	template <typename /*OutCharacter*/, size_t /*StagingGranularity*/, typename InputIterator, typename OutputIterator>
	ETInlineHint ETForceInlineHint OutputIterator TranscodeString(InputIterator begin, InputIterator end, OutputIterator output, TrueType /*isTrivial*/) ETNoexceptHint {
		while (begin != end) {
			*output++ = *begin++;
		}

		return output;
	}

	// ---------------------------------------------------

	template <typename OutCharacter, size_t StagingGranularity, typename InputIterator, typename OutputIterator>
	ETInlineHint ETForceInlineHint OutputIterator TranscodeString(InputIterator begin, InputIterator end, OutputIterator output, FalseType /*isTrivial*/) ETNoexceptHint {
		OutCharacter staging[StagingGranularity];
		while (begin != end) {
			OutCharacter* decoded(staging);
			OutCharacter* last(staging);

			if (!eastl::DecodePart(begin, end, last, staging + StagingGranularity)) {
				break;
			}

			while (decoded != last) {
				*output++ = *decoded++;
			}
		}

		return output;
	}
}

template <typename OutCharacter, size_t StagingGranularity, typename InputIterator, typename OutputIterator>
ETInlineHint ETForceInlineHint OutputIterator TranscodeString(InputIterator begin, InputIterator end, OutputIterator output) ETNoexceptHint {
	using InCharacter = typename RemoveConstVolatile<typename RemoveReference<decltype(*begin)>::Type>::Type;
	return Detail::TranscodeString<OutCharacter, StagingGranularity, InputIterator, OutputIterator>(begin, end, output, IsSame<OutCharacter, InCharacter>());
}

} // namespace Eldritch2
