/*==================================================================*\
  World.cpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game world-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

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
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;

	// ---------------------------------------------------

	AbstractWorld::AbstractWorld(const ObjectInjector& services) ETNoexceptHint : _services(services),
																				  _shouldShutDown(false),
																				  _pauseCounter(0u),
																				  _timeAccumulator(/*targetFrameRateInHz =*/60u, /*timeScale =*/1.0f) {
		// FIXME: This needs to go in a dedicated bind function, it can violate function noexcept contract.
		_services.PublishService(_timeAccumulator);
		_services.PublishService(*this);
	}

	// ---------------------------------------------------

	void AbstractWorld::Tick(JobExecutor& executor) ETNoexceptHint {
		//	Avoid spiral of death by capping the number of fixed-rate ticks that can be run per main loop invocation.
		enum : uint32 { MaxFixedTicksPerInvocation = 3 };

		/*	Execute a variable-rate tick exactly once per main loop iteration. Variable-rate ticks handle real-time
		 *	update tasks like rendering to the screen, mixing sound or polling the network. */
		Stopwatch timer;
		VariableTick(executor, _timeAccumulator.GetTickDurationInMicroseconds(), _timeAccumulator.GetResidualBlendFactor());

		/*	Execute fixed-rate ticks until we are caught up with the real-time clock or no longer have enough time left to account
		 *	for a full frame. The residual time will be carried over into the next @ref World::Tick() invocation(s). */
		for (uint32 tick(0u); ShouldExecute() && tick < MaxFixedTicksPerInvocation; ++tick) {
			if (ShouldSimulate()) {
				_timeAccumulator.AddWallTime(AsMicroseconds(timer.GetDurationAndZero()));
			}

			if (!_timeAccumulator.ShouldSimulate()) {
				break;
			}

			_timeAccumulator.DeductTime();
			FixedTick(executor, _timeAccumulator.GetTickDurationInMicroseconds());
		}
	}

}} // namespace Eldritch2::Core
