/*==================================================================*\
  TimeAccumulator.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/TimeAccumulator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	TimeAccumulator::TimeAccumulator(uint32 targetFrameRateInHz, float32 timeScale) ETNoexceptHint : _residual(MicrosecondsPerFrame(targetFrameRateInHz)) {
		SetFixedTickFramerate(targetFrameRateInHz);
		SetTimeScalar(timeScale);
	}

}} // namespace Eldritch2::Core
