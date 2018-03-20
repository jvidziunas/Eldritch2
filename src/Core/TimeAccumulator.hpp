/*==================================================================*\
  TimeAccumulator.hpp
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

	class TimeAccumulator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref TimeAccumulator instance.
		TimeAccumulator( uint32 targetFrameRateInHz = 60u );
	//!	Constructs this @ref TimeAccumulator instance.
		TimeAccumulator( const TimeAccumulator& ) = default;

		~TimeAccumulator() = default;

	// ---------------------------------------------------

	public:
		uint64	GetResidualTime() const;

		bool	ShouldTick() const;

		void	DeductTime( uint64 amount );
		void	DeductTime();

		void	AddWallTime( uint64 amount );

	// ---------------------------------------------------

	public:
		uint32	GetTickDurationInMicroseconds() const;
		float32	GetTickDurationInSeconds() const;

		uint32	GetTickDurationInWallMicroseconds() const;
		float32	GetTickDurationInWallSeconds() const;

		void	SetFixedTickFramerate( uint32 value );

		float32	GetReciprocalTimeScalar() const;

		float32	GetTimeScalar() const;

		void	SetTimeScalar( float32 value );

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Amount of virtual time to advance the game clock by during a fixed-rate tick, in microseconds.
		uint32	_stepInMicroseconds;
	/*!	Additional time carried over (in the range [0, ULONG_MAX]) after all fixed-length ticks have completed
		for the current frame. */
		uint64	_residual;
	/*!	Reciprocal scaling value (in the range [0, +INF)). The amount of elapsed wall time is scaled by this value
		before being added to the residual. */
		float32	_reciprocalScale;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/TimeAccumulator.inl>
//------------------------------------------------------------------//