/*==================================================================*\
  Listener.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {

	template <typename Voice>
	ETInlineHint Listener<Voice>::Listener() : _activeVoices( MallocAllocator( "Listener Active Voice Collection Allocator" ) ) {}

// ---------------------------------------------------

	template <typename Voice>
	ETInlineHint const typename Listener<Voice>::VoiceList& Listener<Voice>::GetActiveVoices() const {
		return _activeVoices;
	}

// ---------------------------------------------------

	template <typename Voice>
	ETInlineHint float32 Listener<Voice>::GetCurrentLoudnessDb() const {
		return GetLoudnessDb( _activeVoices.Begin(), _activeVoices.End() );
	}

// ---------------------------------------------------

	template <typename Voice>
	ETInlineHint float32 Listener<Voice>::GetWindowTop() const {
		return Max( _perceivedLoudnessDb, _minimumPerceivedLoudnessDb );
	}

// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint float32 GetLoudnessDb( Iterator begin, Iterator end ) {
		float32	sum( 0.0f );

		while (begin != end) {
			sum += Pow( 10.0f, (begin++)->loudnessDb / 10.0f );
		}

		return 10.0f * Log10( sum );
	}

}	// namespace Audio
}	// namespace Eldritch2