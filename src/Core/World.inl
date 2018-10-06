/*==================================================================*\
  World.inl
  ------------------------------------------------------------------
  Purpose:

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	ETInlineHint ETForceInlineHint Range<const UniquePointer<WorldComponent>*> World::GetComponents() const ETNoexceptHint {
		return { _components.Begin(), _components.End() };
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Logging::Log& World::GetLog() const ETNoexceptHint {
		return _log;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float32 World::GetTimeScalar() const ETNoexceptHint {
		return _timeAccumulator.GetTimeScalar();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void World::SetTimeScalar(float32 scalar) ETNoexceptHint {
		_timeAccumulator.SetTimeScalar(scalar);
	}

	// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ETForceInlineHint ErrorCode World::BindResources(Scheduling::JobExecutor& executor, Iterator firstComponent, Iterator lastComponent) {
		ComponentList<> components(_components.GetAllocator());
		components.Reserve(eastl::distance(firstComponent, lastComponent));

		for (; firstComponent != lastComponent; ++firstComponent) {
			UniquePointer<WorldComponent> result((*firstComponent)->CreateWorldComponent(_allocator, _services));
			if (result) {
				components.Append(eastl::move(result));
			}
		}

		components.ShrinkToFit();
		Swap(_components, components);
		return BindResources(executor);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool World::ShouldShutDown(MemoryOrder order) const ETNoexceptHint {
		return _shouldShutDown.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool World::ShouldRun(MemoryOrder order) const ETNoexceptHint {
		return _pauseCounter.load(order) == 0u;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool World::IsRunning(MemoryOrder order) const ETNoexceptHint {
		return _shouldShutDown.load(order) == false;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void World::SetShouldPause(MemoryOrder order) ETNoexceptHint {
		_pauseCounter.fetch_add(1u, order);
	}

}} // namespace Eldritch2::Core
