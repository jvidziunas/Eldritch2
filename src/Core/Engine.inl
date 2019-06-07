/*==================================================================*\
  Engine.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyApiBuilder.hpp>
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	template <typename T, typename = Void_t<>>
	struct WorldComponentType : public Identity<Tuple<>> {};

	// ---

	template <typename>
	struct WorldType;

	// ---

	template <typename T>
	struct WorldComponentType<T, Void_t<typename T::WorldComponent>> : public Identity<Tuple<typename T::WorldComponent>> {};

	// ---

	template <typename... Components>
	struct WorldType<Tuple<Components...>> : public Identity<World<Components...>> {};

	// ---

	template <typename ContentDatabase>
	template <typename... ProviderArgs>
	ETInlineHint ETForceInlineHint AbstractEngine::ManagementComponent<ContentDatabase>::ManagementComponent(const ObjectInjector& services, ProviderArgs&&... args) ETNoexceptHint : EngineComponent(services),
																																													  _content(Forward<ProviderArgs>(args)...),
																																													  _maxSweepsPerFrame(5u) {}

	// ---------------------------------------------------

	template <typename ContentDatabase>
	ETInlineHint void AbstractEngine::ManagementComponent<ContentDatabase>::BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Early-phase initialization", 0xBBBBBB);

		executor.AwaitWork(
			[this, engine = Inject<AbstractEngine>()](Scheduling::JobExecutor& /*executor*/) ETNoexceptHint {
				ET_PROFILE_SCOPE("Engine/Initialization", "Publish asset API", 0xBBBBBB);

				ET_TERMINATE_ENGINE_IF_FAILED(_content.BindResources(engine->BuildApi(AssetApiBuilder())));
			},
			[engine = Inject<AbstractEngine>()](Scheduling::JobExecutor& executor) ETNoexceptHint {
				ET_PROFILE_SCOPE("Engine/Initialization", "Publish configuration", 0xBBBBBB);

				engine->ApplyProperties(executor, SL("Eldritch2.ini"));
			});
	}

	// ---------------------------------------------------

	template <typename ContentDatabase>
	ETInlineHint void AbstractEngine::ManagementComponent<ContentDatabase>::PublishApi(Core::PropertyApiBuilder& api) {
		api.DefineGroup("Engine")
			.DefineProperty("PackageSweepLimitPerFrame", PropertyApiBuilder::MakeSetter(_maxSweepsPerFrame))
			.DefineProperty("LogThreshold", [log = Inject<Logging::Log>()](StringSpan threshold) ETNoexceptHint {
				if (threshold == "Debug") {
					log->SetWriteThreshold(Logging::Severity::Debug);
				} else if (threshold == "VerboseWarning") {
					log->SetWriteThreshold(Logging::Severity::VerboseWarning);
				} else if (threshold == "Warning") {
					log->SetWriteThreshold(Logging::Severity::Warning);
				} else if (threshold == "Error") {
					log->SetWriteThreshold(Logging::Severity::Error);
				} else if (threshold == "Message") {
					log->SetWriteThreshold(Logging::Severity::Message);
				}
			});
	}

	// ---------------------------------------------------

	template <typename ContentDatabase>
	ETInlineHint ETForceInlineHint void AbstractEngine::ManagementComponent<ContentDatabase>::TickEarly(Scheduling::JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Assets", "Destroy asset garbage", 0xBBBBBB);

		_content.DestroyGarbage(_maxSweepsPerFrame);
	}

	// ---------------------------------------------------

	template <typename ContentDatabase>
	ETInlineHint ETForceInlineHint void AbstractEngine::ManagementComponent<ContentDatabase>::Tick(Scheduling::JobExecutor& executor) ETNoexceptHint {
		this->Inject<AbstractEngine>()->TickWorlds(executor);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool AbstractEngine::ShouldRun(MemoryOrder order) const ETNoexceptHint {
		return _shouldRun.load(order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void AbstractEngine::SetShouldShutDown(MemoryOrder order) const ETNoexceptHint {
		_shouldRun.store(false, order);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void AbstractEngine::CreateWorld(Scheduling::JobExecutor& executor) ETNoexceptHint {
		UniquePointer<AbstractWorld> world(this->AllocateWorld(_allocator, _services));
		world->BindResources(executor);

		ET_LOCK_SCOPE(_worldsMutex);
		_worlds.EmplaceBack(Move(world));
		// End of lock scope.
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void AbstractEngine::BootOnCaller(Scheduling::JobExecutor& executor) ETNoexceptHint {
		if (Failed(this->BindResources(executor))) {
			this->SetShouldShutDown();
		}

		while (this->ShouldRun()) {
			this->RunFrame(executor);
		}

		this->FreeResources(executor);
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint Engine<Components...>::Engine() ETNoexceptHint : _components(Tie((ExpandPack<Components>(), _services)...)) {}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint UniquePointer<AbstractWorld> Engine<Components...>::AllocateWorld(Allocator& allocator, const ObjectInjector& services) {
		using ComponentTuple = typename TupleCat<typename WorldComponentType<Components>::Type...>::ResultType;

		return MakeUnique<typename WorldType<ComponentTuple>::Type>(allocator, services);
	}

	// ---------------------------------------------------

	template <typename ApiBuilder>
	ETInlineHint ETForceInlineHint ApiBuilder AbstractEngine::BuildApi(ApiBuilder api) const {
		this->PublishComponents(api);
		return Move(api);
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void Engine<Components...>::BindComponents(Scheduling::JobExecutor& executor) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Initialization", "Bind components", 0xAAAAAA);

		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.BindResourcesEarly(executor);
		});
		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.BindResources(executor);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void Engine<Components...>::FreeComponents(Scheduling::JobExecutor& executor) ETNoexceptHint {
		ET_PROFILE_SCOPE("Engine/Shutdown", "Free components", 0xAAAAAA);

		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.FreeResources(executor);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void Engine<Components...>::BindConfigurableResources(Scheduling::JobExecutor& executor) ETNoexceptHint {
		executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
			component.BindConfigurableResources(executor);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint void Engine<Components...>::RunFrame(Scheduling::JobExecutor& executor) ETNoexceptHint {
		ET_PROFILE_FRAME_BEGIN_CPU();
		ET_PROFILE_SCOPE("Engine/Frame", "Frame", 0xAAAAAA);

		{
			ET_PROFILE_SCOPE("Engine/Frame", "Engine Tick", 0xEEEEEE);
			//	Engine component tick is run in sequence before world tick; we want all shared state to be updated *before* world tick happens.
			executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
				component.TickEarly(executor);
			});
		} // End of profile scope.
		{
			ET_PROFILE_SCOPE("Engine/Frame", "World Tick", 0xFFFFFF);

			executor.ForEach(_components, [](Scheduling::JobExecutor& executor, auto& component) ETNoexceptHint {
				component.Tick(executor);
			});
		} // End of profile scope.
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void Engine<Components...>::PublishComponents(PropertyApiBuilder& api) const {
		ForEach(_components, [&](auto& component) {
			component.PublishApi(api);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void Engine<Components...>::PublishComponents(AssetApiBuilder& api) const {
		ForEach(_components, [&](auto& component) {
			component.PublishApi(api);
		});
	}

	// ---------------------------------------------------

	template <typename... Components>
	ETInlineHint ETForceInlineHint void Engine<Components...>::PublishComponents(ObjectInjector& api) const {
		ForEach(_components, [&](auto& component) {
			component.PublishApi(api);
		});
	}

}} // namespace Eldritch2::Core
