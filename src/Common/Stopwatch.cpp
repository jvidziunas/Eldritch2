/*==================================================================*\
  Stopwatch.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Stopwatch.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

Stopwatch::Stopwatch(CpuTimestamp start) :
	_start(start) {}

// ---------------------------------------------------

Stopwatch::Stopwatch(const Stopwatch& stopwatch) :
	Stopwatch(stopwatch.GetStartTimestamp()) {}

// ---------------------------------------------------

Stopwatch::Stopwatch() :
	Stopwatch(ReadCpuTimestamp()) {}

// ---------------------------------------------------

CpuTimestamp Stopwatch::GetDurationAndZero() {
	const auto start(ReadCpuTimestamp());
	return start - _start.exchange(start, std::memory_order_relaxed);
}

// ---------------------------------------------------

CpuTimestamp Stopwatch::GetDuration() const {
	return ReadCpuTimestamp() - GetStartTimestamp();
}

// ---------------------------------------------------

CpuTimestamp Stopwatch::GetStartTimestamp() const {
	return _start.load(std::memory_order_relaxed);
}

// ---------------------------------------------------

Stopwatch& Stopwatch::operator=(const Stopwatch& stopwatch) {
	_start.store(stopwatch.GetStartTimestamp(), std::memory_order_relaxed);

	return *this;
}

} // namespace Eldritch2
