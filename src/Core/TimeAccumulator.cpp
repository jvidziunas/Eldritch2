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

		static ETInlineHint ETPureFunctionHint uint32 MicrosecondsPerFrame(uint32 framerateInHz) {
			return MicrosecondsPerSecond / Clamp<uint32>(framerateInHz, 1u, MicrosecondsPerSecond);
		}

		// ---------------------------------------------------

		static ETInlineHint ETPureFunctionHint float32 ZeroSafeReciprocal(float32 value) {
			return value == 0.0f ? 0.0f : Reciprocal(value);
		}

	} // anonymous namespace

	TimeAccumulator::TimeAccumulator(
		uint32  targetFrameRateInHz,
		float32 timeScale) :
		_residual(MicrosecondsPerFrame(targetFrameRateInHz)) {
		SetFixedTickFramerate(targetFrameRateInHz);
		SetTimeScalar(timeScale);
	}

	// ---------------------------------------------------

	uint32 TimeAccumulator::GetTickDurationInMicroseconds() const {
		return _stepInMicroseconds;
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetTickDurationInSeconds() const {
		return AsFloat(_stepInMicroseconds) / AsFloat(MicrosecondsPerSecond);
	}

	// ---------------------------------------------------

	uint32 TimeAccumulator::GetTickDurationInWallMicroseconds() const {
		return static_cast<uint32>(AsInt(_reciprocalScale * _stepInMicroseconds));
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetTickDurationInWallSeconds() const {
		return (_reciprocalScale * _stepInMicroseconds) / AsFloat(MicrosecondsPerSecond);
	}

	// ---------------------------------------------------

	void TimeAccumulator::SetFixedTickFramerate(uint32 value) {
		_stepInMicroseconds = MicrosecondsPerFrame(value);
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetReciprocalTimeScalar() const {
		return _reciprocalScale;
	}

	// ---------------------------------------------------

	float32 TimeAccumulator::GetTimeScalar() const {
		return ZeroSafeReciprocal(_reciprocalScale);
	}

	// ---------------------------------------------------

	void TimeAccumulator::SetTimeScalar(float32 value) {
		_reciprocalScale = ZeroSafeReciprocal(Clamp(value, 0.0f, 100.0f));
	}

}} // namespace Eldritch2::Core
