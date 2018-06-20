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
		namespace {

			enum : size_t {
				HighParallelismSplit = 1u,
				LowParallelismSplit = 2u
			};

			enum : uint32 { DefaultFixedTickFramerate = 60u };

		// ---

			ETInlineHint ETPureFunctionHint float32 AsMilliseconds(uint64 microseconds) {
				static constexpr float32	MicrosecondsPerMillisecond = 1000.0f;

				return microseconds / MicrosecondsPerMillisecond;
			}

		}	// anonymous namespace

		using namespace ::Eldritch2::Scheduling;
		using namespace ::Eldritch2::Scripting;
		using namespace ::Eldritch2::Logging;
		using namespace ::Eldritch2::Assets;

	// ---------------------------------------------------

		World::World(
			const Blackboard& services,
			Log& log
		) : _allocator("World Root Allocator"),
			_log(log),
			_shouldShutDown(false),
			_pauseCounter(0u),
			_services(services),
			_timeAccumulator(DefaultFixedTickFramerate, 1.0f),
			_components(MallocAllocator("World Services Collection Allocator")) {
			_services.Publish<World>(*this);
		}

	// ---------------------------------------------------

		void World::Tick(JobExecutor& executor) {
		//	Avoid spiral of death by capping the number of fixed-rate ticks that can be run per main loop invocation.
			enum : uint32 { MaxFixedTicksPerInvocation = 3 };

			Stopwatch	wallTimer;

		/*	Execute a variable-rate tick exactly once per main loop iteration. Variable-rate ticks handle real-time
		 *	update tasks like rendering to the screen, mixing sound or polling the network. */
			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [residualTime = _timeAccumulator.GetResidualTime()](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->AcceptVisitor(executor, WorldComponent::VariableTickVisitor(residualTime));
			});

		/*	Execute fixed-rate ticks until we are caught up with the real-time clock or no longer have enough time left to account
		 *	for a full frame. The residual time will be carried over into the next @ref World::Tick() invocation(s). */
			for (uint32 tick(0u); IsRunning() && tick < MaxFixedTicksPerInvocation; ++tick) {
				if (ShouldPause() == false) {
					_timeAccumulator.AddWallTime(wallTimer.GetDurationAndZero());
				}

				if (!_timeAccumulator.ShouldTick()) {
					break;
				}

				const uint32 duration(_timeAccumulator.GetTickDurationInMicroseconds());

				_timeAccumulator.DeductTime(duration);

				executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [duration](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
					component->AcceptVisitor(executor, WorldComponent::EarlyTickVisitor(duration));
				});
				executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [duration](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
					component->AcceptVisitor(executor, WorldComponent::StandardTickVisitor(duration));
				});
				executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [duration](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
					component->AcceptVisitor(executor, WorldComponent::LateTickVisitor(duration));
				});
			}
		}

	// ---------------------------------------------------

		ErrorCode World::BindResources(JobExecutor& executor) {
			Stopwatch	timer;

			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->AcceptVisitor(executor, WorldComponent::EarlyInitializationVisitor());
			});
			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->AcceptVisitor(executor, WorldComponent::LateInitializationVisitor());
			});

		//	Components indicate unsuccessful initialization by requesting that the world shut down.
			if (!ShouldShutDown()) {
				_log.Write(MessageType::Message, "Initialized world {} in {:.2f}ms." UTF8_NEWLINE, fmt::ptr(this), AsMilliseconds(timer.GetDuration()));
			}

			return Error::None;
		}

	// ---------------------------------------------------

		void World::FreeResources(JobExecutor& executor) {
			executor.ForEach<HighParallelismSplit>(_components.Begin(), _components.End(), [](JobExecutor& executor, UniquePointer<WorldComponent>& component) {
				component->AcceptVisitor(executor, WorldComponent::TearDownVisitor());
			});

			_components.Clear(ReleaseMemorySemantics());
		}

	// ---------------------------------------------------

		void World::SetShouldShutDown(bool andEngine) const {
			_shouldShutDown.store(true, std::memory_order_release);

			if (andEngine) {
				_services.Find<Engine>().SetShouldShutDown();
			}
		}

	// ---------------------------------------------------

		void World::SetShouldPause() {
			_pauseCounter.fetch_add(1u, std::memory_order_relaxed);
		}

	}	// namespace Core
}	// namespace Eldritch2