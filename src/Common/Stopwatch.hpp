/*==================================================================*\
  Stopwatch.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Platform.hpp>
#include <Common/Atomic.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

class Stopwatch {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Stopwatch instance.
	Stopwatch(CpuTimestamp start) ETNoexceptHint;
	//!	Constructs this @ref Stopwatch instance.
	Stopwatch(const Stopwatch&) ETNoexceptHint;
	//!	Constructs this @ref Stopwatch instance.
	Stopwatch() ETNoexceptHint;

	~Stopwatch() = default;

	// ---------------------------------------------------

public:
	CpuTimestamp GetDurationAndZero() ETNoexceptHint;

	CpuTimestamp GetDuration() const ETNoexceptHint;

	float32 GetDurationMillisecondsAndZero() ETNoexceptHint;

	float32 GetDurationMilliseconds() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	CpuTimestamp GetStartTimestamp() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	Stopwatch& operator=(const Stopwatch&) ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	Atomic<CpuTimestamp> _start;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Stopwatch.inl>
//------------------------------------------------------------------//