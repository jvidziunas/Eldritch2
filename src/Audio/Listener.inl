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

namespace Eldritch2 { namespace Audio {

	template <typename Voice>
	ETInlineHint ETForceInlineHint Listener<Voice>::Listener(Transformation localToWorld, Vector linearVelocity) ETNoexceptHint : _voices(VoiceList::AllocatorType("Listener Active Voice List Allocator")),
																																  _localToWorld(localToWorld),
																																  _linearVelocity(linearVelocity),
																																  _perceivedLoudnessDb(60.0f), /* Close to normal conversational volume */
																																  _minimumPerceivedLoudnessDb(30.0f) /* Whisper level w/ minimal background ambience. */ {}

	// ---------------------------------------------------

	template <typename Voice>
	ETConstexpr ETForceInlineHint const typename Listener<Voice>::VoiceList& Listener<Voice>::GetActiveVoices() const ETNoexceptHint {
		return _voices;
	}

	// ---------------------------------------------------

	template <typename Voice>
	ETConstexpr ETForceInlineHint Transformation ETSimdCall Listener<Voice>::GetLocalToWorld() const ETNoexceptHint {
		return _localToWorld;
	}

	// ---------------------------------------------------

	template <typename Voice>
	ETConstexpr ETForceInlineHint Vector ETSimdCall Listener<Voice>::GetLinearVelocity() const ETNoexceptHint {
		return _linearVelocity;
	}

	// ---------------------------------------------------

	template <typename Voice>
	ETInlineHint ETForceInlineHint void ETSimdCall Listener<Voice>::MoveTo(Transformation localToWorld, Vector linearVelocity) ETNoexceptHint {
		_localToWorld   = localToWorld;
		_linearVelocity = linearVelocity;
	}

	// ---------------------------------------------------

	template <typename Voice>
	ETInlineHint ETForceInlineHint float32 Listener<Voice>::GetCurrentDiegeticLoudnessDb() const ETNoexceptHint {
		return GetDiegeticLoudnessDb(_voices.Begin(), _voices.End());
	}

	// ---------------------------------------------------

	template <typename Voice>
	ETConstexpr ETForceInlineHint float32 Listener<Voice>::GetWindowTopDb() const ETNoexceptHint {
		return Maximum(_perceivedLoudnessDb, _minimumPerceivedLoudnessDb);
	}

	// ---------------------------------------------------

	template <typename InputIterator>
	ETInlineHint ETForceInlineHint float32 GetDiegeticLoudnessDb(InputIterator begin, InputIterator end) ETNoexceptHint {
		return 10.0f * Log10(Reduce(begin, end, 0.0f, [](decltype(*begin) voice, float32 loudnessDb) ETNoexceptHint {
			return loudnessDb + Pow(10.0f, voice.loudnessDb * 0.1f);
		}));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint ETPureFunctionHint float32 GetHdrVolumeScalar(float32 windowTopDb, float32 volumeDb) ETNoexceptHint {
		return Pow(10.0f, (volumeDb - windowTopDb) * 0.05f);
	}

}} // namespace Eldritch2::Audio
