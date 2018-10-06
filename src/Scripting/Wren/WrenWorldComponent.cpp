/*==================================================================*\
  WrenWorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenWorldComponent.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Assets/ContentLocator.hpp>
#include <Assets/AssetDatabase.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

#define ET_TERMINATE_WORLD_UNLESS(cond)                  \
	if (!(cond)) {                                       \
		FindService<Core::World>()->SetShouldShutDown(); \
	}

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	WrenWorldComponent::WrenWorldComponent(const ObjectLocator& services) :
		WorldComponent(services),
		_wren(),
		_contentLocator(nullptr),
		_requiredPackages(nullptr),
		_dispatcher(nullptr),
		_game(nullptr) {}

	// ---------------------------------------------------

	void WrenWorldComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/EarlyInitialization", "Wren API registration", 0xCBABCB);
		using ::Eldritch2::Swap;

		Context wren;
		ET_TERMINATE_WORLD_UNLESS(Succeeded(wren.BindResources(*FindService<AssetDatabase>(), FindService<World>()->GetLog(), [this](ApiBuilder& api) {
			for (const UniquePointer<WorldComponent>& component : FindService<World>()->GetComponents()) {
				component->DefineScriptApi(api);
			}
		})));

		Swap(_wren, wren);
	}

	// ---------------------------------------------------

	void WrenWorldComponent::BindResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("World/Initialization", "Wren world boot", 0xCBABCB);

		ET_TERMINATE_WORLD_UNLESS(_contentLocator && Succeeded(_contentLocator->BindResources(*FindService<AssetDatabase>(), *FindService<PackageDatabase>())));
		ET_TERMINATE_WORLD_UNLESS(_dispatcher && Succeeded(_dispatcher->BindResources(_wren, FindService<World>()->GetTimeScalar())));
		ET_TERMINATE_WORLD_UNLESS(_game && Succeeded(_game->BindResources(_wren, *_dispatcher)));
	}

	// ---------------------------------------------------

	void WrenWorldComponent::FreeResources(JobExecutor& executor) {
		if (Game* const game = eastl::exchange(_game, nullptr)) {
			game->FreeResources(_wren);
		}

		if (Dispatcher* const dispatcher = eastl::exchange(_dispatcher, nullptr)) {
			dispatcher->FreeResources(_wren);
		}

		if (AsynchronousImport* const requiredPackages = eastl::exchange(_requiredPackages, nullptr)) {
			executor.AwaitCondition(requiredPackages->IsComplete());
		}

		if (ContentLocator* const locator = eastl::exchange(_contentLocator, nullptr)) {
			locator->FreeResources();
		}

		_wren.FreeResources();
	}

	// ---------------------------------------------------

	void WrenWorldComponent::OnVariableRateTick(JobExecutor& /*executor*/, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);
		// _contentLocator->RequirementsComplete();
	}

	// ---------------------------------------------------

	void WrenWorldComponent::OnFixedRateTick(JobExecutor& /*executor*/, MicrosecondTime delta) {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);
		_dispatcher->ExecuteScriptEvents(_wren, delta);

		World* const world(FindService<World>());
		if (_dispatcher->ShouldShutDownWorld()) {
			world->SetShouldShutDown(/*andEngine =*/false);
		}

		world->SetTimeScalar(_dispatcher->GetTimeScalar());
	}

}}} // namespace Eldritch2::Scripting::Wren
