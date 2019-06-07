/*==================================================================*\
  TimeAccumulator.inl
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

namespace Eldritch2 { namespace Core {

	ETConstexpr ETForceInlineHint MicrosecondTime TimeAccumulator::GetTickDurationInMicroseconds() const ETNoexceptHint {
		return MicrosecondTime(_stepInMicroseconds);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 TimeAccumulator::GetResidualBlendFactor() const ETNoexceptHint {
		const MicrosecondTime frameEnd(GetTickDurationInWallMicroseconds());
		const MicrosecondTime coordinate(Minimum(GetResidualTime(), frameEnd));

		return float32(coordinate) / float32(frameEnd);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint MicrosecondTime TimeAccumulator::GetResidualTime() const ETNoexceptHint {
		return _residual;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 TimeAccumulator::GetTickDurationInSeconds() const ETNoexceptHint {
		return float32(_stepInMicroseconds) / float32(MicrosecondsPerSecond);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint MicrosecondTime TimeAccumulator::GetTickDurationInWallMicroseconds() const ETNoexceptHint {
		return MicrosecondTime(uint64(_reciprocalScale * uint64(_stepInMicroseconds)));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 TimeAccumulator::GetTickDurationInWallSeconds() const ETNoexceptHint {
		return uint64(GetTickDurationInWallMicroseconds()) / float32(MicrosecondsPerSecond);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool TimeAccumulator::ShouldSimulate() const ETNoexceptHint {
		return GetTickDurationInWallMicroseconds() <= GetResidualTime();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void TimeAccumulator::DeductTime(MicrosecondTime amount) ETNoexceptHint {
		_residual -= amount;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void TimeAccumulator::DeductTime() ETNoexceptHint {
		DeductTime(GetTickDurationInWallMicroseconds());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void TimeAccumulator::AddWallTime(MicrosecondTime amount) ETNoexceptHint {
		_residual += MicrosecondTime(uint64(uint64(amount) * _reciprocalScale));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint void TimeAccumulator::SetFixedTickFramerate(uint32 value) ETNoexceptHint {
		_stepInMicroseconds = MicrosecondsPerFrame(value);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float32 TimeAccumulator::GetReciprocalTimeScalar() const ETNoexceptHint {
		return _reciprocalScale;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float32 TimeAccumulator::GetTimeScalar() const ETNoexceptHint {
		return ZeroSafeReciprocal(GetReciprocalTimeScalar());
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void TimeAccumulator::SetTimeScalar(float32 value) ETNoexceptHint {
		_reciprocalScale = ZeroSafeReciprocal(Clamp(value, 0.0f, 100.0f));
	}

}} // namespace Eldritch2::Core
