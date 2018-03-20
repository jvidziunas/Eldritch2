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

namespace Eldritch2 {
namespace Core {
namespace {

	enum : uint32 {
		MicrosecondsPerSecond = 1000000u
	};

// ---

	static ETInlineHint ETPureFunctionHint uint32 TimestepFromFramerate( uint32 framerateInHz ) {
		return MicrosecondsPerSecond / Clamp<uint32>( framerateInHz, 1u, MicrosecondsPerSecond );
	}

}	// anonymous namespace

	TimeAccumulator::TimeAccumulator(
		uint32 targetFrameRateInHz
	) : _residual( TimestepFromFramerate( targetFrameRateInHz ) ),
		_reciprocalScale( 1.0f ) {
		SetFixedTickFramerate( targetFrameRateInHz );
	}

// ---------------------------------------------------

	uint32 TimeAccumulator::GetTickDurationInMicroseconds() const {
		return _stepInMicroseconds;
	}

// ---------------------------------------------------

	float32 TimeAccumulator::GetTickDurationInSeconds() const {
		return AsFloat( _stepInMicroseconds ) / AsFloat( MicrosecondsPerSecond );
	}

// ---------------------------------------------------

	uint32 TimeAccumulator::GetTickDurationInWallMicroseconds() const {
		return static_cast<uint32>(AsInt( _reciprocalScale * _stepInMicroseconds ));
	}

// ---------------------------------------------------

	float32 TimeAccumulator::GetTickDurationInWallSeconds() const {
		return (_reciprocalScale * _stepInMicroseconds) / AsFloat( MicrosecondsPerSecond );
	}

// ---------------------------------------------------

	void TimeAccumulator::SetFixedTickFramerate( uint32 value ) {
		_stepInMicroseconds = TimestepFromFramerate( value );
	}

// ---------------------------------------------------

	float32 TimeAccumulator::GetReciprocalTimeScalar() const {
		return _reciprocalScale;
	}

// ---------------------------------------------------

	float32 TimeAccumulator::GetTimeScalar() const {
		const float32	inverseScalar( _reciprocalScale );

		return ( inverseScalar != 0.0f ? Reciprocal( inverseScalar ) : 0.0f );
	}

// ---------------------------------------------------

	void TimeAccumulator::SetTimeScalar( float32 value ) {
		const float32	clampedValue( Clamp( value, 0.0f, 100.0f ) );

	//	Avoid division by 0.
		_reciprocalScale = ( clampedValue != 0.0f ? Reciprocal( clampedValue ) : 0.0f );
	}

}	// namespace Core
}	// namespace Eldritch2