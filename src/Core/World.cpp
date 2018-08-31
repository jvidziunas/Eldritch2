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
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Assets/PackageDatabase.hpp>
#include <Core/Engine.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	namespace {

		enum : size_t {
			HighParallelismSplit = 1u,
			LowParallelismSplit  = 2u
		};

		// ---

		ETInlineHint ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime microseconds) {
			return AsFloat(microseconds) / /*microseconds per millisecond*/ 1000.0f;
		}

	} // anonymous namespace

	World::World(const ObjectLocator& services) :
		_allocator("World Root Allocator"),
		_services(services),
		_log(services.Find<Engine>()->GetLog()),
		_shouldShutDown(false),
		_pauseCounter(0u),
		_timeAccumulator(/*default fixed tick framerate*/ 60u, 1.0f),
		_components(MallocAllocator("World Services Collection Allocator")) {
		_services.PublishService<World>(*this);
	}

	// ---------------------------------------------------

	void World::Tick(JobExecutor& executor) {
		//	Avoid spiral of death by capping the number of fixed-rate ticks that can be run per main loop invocation.
		enum : uint32 { MaxFixedTicksPerInvocation = 3 };

		Stopwatch timer;
		/*	Execute a variable-rate tick exactly once per main loop iteration. Variable-rate ticks handle real-time
		 *	update tasks like rendering to the screen, mixing sound or polling the network. */
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [this](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
			component->OnVariableRateTick(executor, _timeAccumulator.GetTickDurationInMicroseconds(), _timeAccumulator.GetResidualBlendFactor());
		});

		/*	Execute fixed-rate ticks until we are caught up with the real-time clock or no longer have enough time left to account
		 *	for a full frame. The residual time will be carried over into the next @ref World::Tick() invocation(s). */
		for (uint32 tick(0u); IsRunning() && tick < MaxFixedTicksPerInvocation; ++tick) {
			if (ShouldRun()) {
				_timeAccumulator.AddWallTime(timer.GetDurationAndZero());
			}
			if (!_timeAccumulator.ShouldTick()) {
				break;
			}

			const MicrosecondTime delta(_timeAccumulator.GetTickDurationInMicroseconds());
			_timeAccumulator.DeductTime();

			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [delta](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->OnFixedRateTickEarly(executor, delta);
			});
			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [delta](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->OnFixedRateTick(executor, delta);
			});
			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [delta](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->OnFixedRateTickLate(executor, delta);
			});
		}
	}

	// ---------------------------------------------------

	ErrorCode World::BindResources(JobExecutor& executor) {
		Stopwatch timer;
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
			component->BindResourcesEarly(executor);
		});
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
			component->BindResources(executor);
		});

		if (ShouldShutDown()) {
			//	Components indicate unsuccessful initialization by requesting that the world shut down.
			return Error::Unspecified;
		}

		_log.Write(Severity::Message, "Initialized world {} in {:.2f}ms." ET_NEWLINE, fmt::ptr(this), AsMilliseconds(timer.GetDuration()));
		return Error::None;
	}

	// ---------------------------------------------------

	void World::FreeResources(JobExecutor& executor) {
		executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
			component->FreeResources(executor);
		});

		_components.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	void World::SetShouldShutDown(bool andEngine) const {
		_shouldShutDown.store(true, std::memory_order_release);
		if (andEngine) {
			_services.Find<Engine>()->SetShouldShutDown();
		}
	}

}} // namespace Eldritch2::Core
