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
//------------------------------------------------------------------//

namespace Eldritch2 {

	class Stopwatch {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Stopwatch instance.
		Stopwatch( CpuTimestamp start );
	//!	Constructs this @ref Stopwatch instance.
		Stopwatch( const Stopwatch& );
	//!	Constructs this @ref Stopwatch instance.
		Stopwatch();

		~Stopwatch() = default;

	// ---------------------------------------------------

	public:
		CpuTimestamp	GetDurationAndZero();
		
		CpuTimestamp	GetDuration() const;

	// ---------------------------------------------------

	public:
		CpuTimestamp	GetStartTimestamp() const;

	// ---------------------------------------------------

	public:
		Stopwatch&	operator=( const Stopwatch& );

	// - DATA MEMBERS ------------------------------------

	private:
		Atomic<CpuTimestamp>	_start;
	};

}	// namespace Eldritch2