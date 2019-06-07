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
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	ETInlineHint ETForceInlineHint void AbstractWorld::SetShouldShutDown(MemoryOrder order) const ETNoexceptHint {
		_shouldShutDown.store(true, order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool AbstractWorld::ShouldShutDown(MemoryOrder order) const ETNoexceptHint {
		return _shouldShutDown.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool AbstractWorld::ShouldExecute(MemoryOrder order) const ETNoexceptHint {
		return _shouldShutDown.load(order) == false;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool AbstractWorld::ShouldSimulate(MemoryOrder order) const ETNoexceptHint {
		return _pauseCounter.load(order) != 0u;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void AbstractWorld::SetShouldPause(MemoryOrder order) ETNoexceptHint {
		_pauseCounter.fetch_add(1u, order);
	}

	// ---------------------------------------------------

	template <typename ApiBuilder>
	ETInlineHint ETForceInlineHint ApiBuilder AbstractWorld::BuildApi(ApiBuilder api) const {
		this->PublishComponents(api);
		return Move(api);
	}

	// ---------------------------------------------------

	// Pack expansion is a little unusual to allow for in-place construction of world components/relax move-constructible constraint on component types.
	template <typename... Components>
	ETInlineHint ETForceInlineHint World<Components...>::World(const ObjectInjector& services) ETNoexceptHint : AbstractWorld(services), _components(Tie((ExpandPack<Components>(), _services)...)) {}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void World<Components...>::PublishComponents(Scripting::Wren::ApiBuilder& api) const {
		ForEach(_components, [&](auto& component) {
			component.PublishApi(api);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void World<Components...>::PublishComponents(ObjectInjector& api) const {
		ForEach(_components, [&](auto& component) {
			component.PublishApi(api);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void World<Components...>::BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint {
		PublishComponents(_services);

		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.BindResourcesEarly(executor);
		});
		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.BindResources(executor);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void World<Components...>::FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint {
		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.FreeResources(executor);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void World<Components...>::VariableTick(Scheduling::JobExecutor& executor, MicrosecondTime duration, float32 blendFraction) ETNoexceptHint {
		executor.ForEach(_components, [=](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.OnVariableRateTick(executor, duration, blendFraction);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void World<Components...>::FixedTick(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint {
		executor.ForEach(_components, [=](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.OnFixedRateTickEarly(executor, duration);
		});
		executor.ForEach(_components, [=](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.OnFixedRateTick(executor, duration);
		});
		executor.ForEach(_components, [=](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.OnFixedRateTickLate(executor, duration);
		});
	}

}} // namespace Eldritch2::Core
