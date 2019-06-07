/*==================================================================*\
  Stopwatch.cpp
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

//------------------------------------------------------------------//

namespace Eldritch2 {

Stopwatch::Stopwatch(CpuTimestamp start) ETNoexceptHint : _start(start) {}

// ---------------------------------------------------

Stopwatch::Stopwatch(const Stopwatch& stopwatch) ETNoexceptHint : Stopwatch(stopwatch.GetStartTimestamp()) {}

// ---------------------------------------------------

Stopwatch::Stopwatch() ETNoexceptHint : Stopwatch(ReadCpuTimestamp()) {}

// ---------------------------------------------------

CpuTimestamp Stopwatch::GetDurationAndZero() ETNoexceptHint {
	const auto start(ReadCpuTimestamp());
	return CpuTimestamp(uint64(start) - uint64(_start.exchange(start, std::memory_order_relaxed)));
}

// ---------------------------------------------------

CpuTimestamp Stopwatch::GetDuration() const ETNoexceptHint {
	return CpuTimestamp(uint64(ReadCpuTimestamp()) - uint64(GetStartTimestamp()));
}

// ---------------------------------------------------

CpuTimestamp Stopwatch::GetStartTimestamp() const ETNoexceptHint {
	return _start.load(std::memory_order_relaxed);
}

// ---------------------------------------------------

Stopwatch& Stopwatch::operator=(const Stopwatch& stopwatch) ETNoexceptHint {
	_start.store(stopwatch.GetStartTimestamp(), std::memory_order_relaxed);
	return *this;
}

} // namespace Eldritch2
