/*==================================================================*\
  Engine.ManagementComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Core/PropertyRegistrar.hpp>
#include <Assets/AssetDatabase.hpp>
#include <Core/Engine.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;

	namespace {

		ETInlineHint ETPureFunctionHint float32 AsMilliseconds(MicrosecondTime delta) {
			static constexpr float32 MicrosecondsPerMillisecond = 1000.0f;
			return AsFloat(delta) / MicrosecondsPerMillisecond;
		}

	} // anonymous namespace

	Engine::ManagementComponent::ManagementComponent(
		Engine& owner) :
		EngineComponent(owner.GetServiceLocator()),
		_owner(eastl::addressof(owner)),
		_packageSweepLimitPerFrame(5u) {
	}

	// ---------------------------------------------------

	void Engine::ManagementComponent::BindResourcesEarly(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Early-phase initialization", 0xBBBBBB);
		/*	Attach component services for dependency injection. This should be done *before* any asynchronous work since
		 *	we don't statically know what services our components will request from one another during initialization. */
		for (EngineComponent* component : _owner->_components) {
			component->PublishServices(_owner->_services);
		}

		executor.AwaitWork(
			[this](JobExecutor& executor) {
				ET_PROFILE_SCOPE("Engine/Initialization", "Load configuration", 0xBBBBBB);
				const ArrayList<EngineComponent*>& components(_owner->_components);

				_owner->ApplyConfiguration(L"Eldritch2.ini");
				executor.ForEach<2u>(components.Begin(), components.End(), [](JobExecutor& executor, EngineComponent* component) {
					component->BindConfigurableResources(executor);
				});
			},
			[this](JobExecutor& /*executor*/) {
				ET_PROFILE_SCOPE("Engine/Initialization", "Build asset API", 0xBBBBBB);
				AssetApiBuilder api;

				for (EngineComponent* component : _owner->_components) {
					component->PublishAssetTypes(api);
				}

				_owner->_packageProvider.GetAssetDatabase().BindResources(eastl::move(api.GetFactories()));
			},
			[this](JobExecutor& /*executor*/) {
				ET_PROFILE_SCOPE("Engine/Initialization", "Scan packages", 0xBBBBBB);
				Stopwatch timer;

				_owner->_packageProvider.ScanPackages();
				_owner->_log.Write(MessageType::Message, "Rebuilt package listing in {:.2f}ms." UTF8_NEWLINE, AsMilliseconds(timer.GetDuration()));
			});
	}

	// ---------------------------------------------------

	void Engine::ManagementComponent::TickEarly(JobExecutor& /*executor*/) {
		_owner->SweepPackages(_packageSweepLimitPerFrame);
	}

	// ---------------------------------------------------

	void Engine::ManagementComponent::Tick(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Engine/WorldTick", "Tick worlds", 0xBBBBBB);
		_owner->TickWorlds(executor);
	}

	// ---------------------------------------------------

	void Engine::ManagementComponent::PublishConfiguration(PropertyRegistrar& properties) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Core property registration", 0xBBBBBB);
		Log& log(_owner->GetLog());

		properties.BeginSection("Engine")
			.DefineProperty("PackageSweepLimitPerFrame", _packageSweepLimitPerFrame)
			.DefineProperty("LogThreshold", [&](StringView<Utf8Char> threshold) {
				if (threshold == "VerboseWarning") {
					log.SetWriteThreshold(MessageType::VerboseWarning);
				} else if (threshold == "Warning") {
					log.SetWriteThreshold(MessageType::Warning);
				} else if (threshold == "Error") {
					log.SetWriteThreshold(MessageType::Error);
				} else if (threshold == "Message") {
					log.SetWriteThreshold(MessageType::Message);
				}
			});
	}

}} // namespace Eldritch2::Core
