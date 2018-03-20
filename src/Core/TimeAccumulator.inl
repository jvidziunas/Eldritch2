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

namespace Eldritch2 {
namespace Core {

	ETInlineHint uint64 TimeAccumulator::GetResidualTime() const {
		return _residual;
	}

// ---------------------------------------------------

	ETInlineHint bool TimeAccumulator::ShouldTick() const {
		return _residual <= GetTickDurationInWallMicroseconds();
	}

// ---------------------------------------------------

	ETInlineHint void TimeAccumulator::DeductTime( uint64 amount ) {
		_residual -= amount;
	}

// ---------------------------------------------------

	ETInlineHint void TimeAccumulator::DeductTime() {
		DeductTime( GetTickDurationInWallMicroseconds() );
	}

// ---------------------------------------------------

	ETInlineHint void TimeAccumulator::AddWallTime( uint64 amount ) {
		_residual += AsInt( amount * _reciprocalScale );
	}

}	// namespace Core
}	// namespace Eldritch2