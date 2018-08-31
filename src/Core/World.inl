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

	ETInlineHint const World::ComponentList<>& World::GetComponents() const {
		return _components;
	}

	// ---------------------------------------------------

	ETInlineHint Logging::Log& World::GetLog() const {
		return _log;
	}

	// ---------------------------------------------------

	ETInlineHint float32 World::GetTimeScalar() const {
		return _timeAccumulator.GetTimeScalar();
	}

	// ---------------------------------------------------

	ETInlineHint void World::SetTimeScalar(float32 scalar) {
		_timeAccumulator.SetTimeScalar(scalar);
	}

	// ---------------------------------------------------

	template <typename Iterator>
	ETInlineHint ErrorCode World::BindResources(Scheduling::JobExecutor& executor, Iterator firstComponent, Iterator lastComponent) {
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

	ETInlineHint bool World::ShouldShutDown(MemoryOrder order) const {
		return _shouldShutDown.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint bool World::ShouldRun(MemoryOrder order) const {
		return _pauseCounter.load(order) == 0u;
	}

	// ---------------------------------------------------

	ETInlineHint bool World::IsRunning(MemoryOrder order) const {
		return _shouldShutDown.load(order) == false;
	}

	// ---------------------------------------------------

	ETInlineHint void World::SetShouldPause(MemoryOrder order) {
		_pauseCounter.fetch_add(1u, order);
	}

}} // namespace Eldritch2::Core
