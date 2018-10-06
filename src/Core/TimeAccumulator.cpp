/*==================================================================*\
  TimeAccumulator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/TimeAccumulator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	namespace {

		enum : uint32 { MicrosecondsPerSecond = 1000000u };

		// ---

		ETInlineHint ETForceInlineHint ETPureFunctionHint uint32 MicrosecondsPerFrame(uint32 framerateInHz) ETNoexceptHint {
			return MicrosecondsPerSecond / Clamp<uint32>(framerateInHz, 1u, MicrosecondsPerSecond);
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ETPureFunctionHint float32 ZeroSafeReciprocal(float32 value) ETNoexceptHint {
			return value == 0.0f ? 0.0f : Reciprocal(value);
		}

	} // anonymous namespace

	TimeAccumulator::TimeAccumulator(uint32 targetFrameRateInHz, float32 timeScale) ETNoexceptHint : _residual(MicrosecondsPerFrame(targetFrameRateInHz)) {
		SetFixedTickFramerate(targetFrameRateInHz);
		SetTimeScalar(timeScale);
	}

	// ---------------------------------------------------

	uint32 TimeAccumulator::GetTickDurationInMicroseconds() const ETNoexceptHint {
		return _stepInMicroseconds;
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetTickDurationInSeconds() const ETNoexceptHint {
		return AsFloat(_stepInMicroseconds) / AsFloat(MicrosecondsPerSecond);
	}

	// ---------------------------------------------------

	uint32 TimeAccumulator::GetTickDurationInWallMicroseconds() const ETNoexceptHint {
		return uint32(AsInt(_reciprocalScale * _stepInMicroseconds));
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetTickDurationInWallSeconds() const ETNoexceptHint {
		return (_reciprocalScale * _stepInMicroseconds) / AsFloat(MicrosecondsPerSecond);
	}

	// ---------------------------------------------------

	void TimeAccumulator::SetFixedTickFramerate(uint32 value) ETNoexceptHint {
		_stepInMicroseconds = MicrosecondsPerFrame(value);
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetReciprocalTimeScalar() const ETNoexceptHint {
		return _reciprocalScale;
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetTimeScalar() const ETNoexceptHint {
		return ZeroSafeReciprocal(_reciprocalScale);
	}

	// ---------------------------------------------------

	void TimeAccumulator::SetTimeScalar(float32 value) ETNoexceptHint {
		_reciprocalScale = ZeroSafeReciprocal(Clamp(value, 0.0f, 100.0f));
	}

}} // namespace Eldritch2::Core
