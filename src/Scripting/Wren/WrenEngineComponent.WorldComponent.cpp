/*==================================================================*\
  WrenEngineComponent.WorldComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Scripting/Wren/Dispatcher.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scheduling/JobExecutor.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/ContentLocator.hpp>
#include <Core/Profiler.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	WrenEngineComponent::WorldComponent::WorldComponent(const ObjectInjector& services) ETNoexceptHint : Core::WorldComponent(services),
																										 _content(nullptr),
																										 _worldPackages(nullptr),
																										 _dispatcher(nullptr),
																										 _game(nullptr) {}

	// ---------------------------------------------------

	void WrenEngineComponent::WorldComponent::BindResourcesEarly(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/EarlyInitialization", "Wren API registration", 0xCBABCB);

		ApiBuilder api(_wren.CreateApiBuilder(/*heapByteSize =*/2u * 1024u * 1024u));
		ET_TERMINATE_WORLD_IF_FAILED(_wren.BindResources(*Inject<Log>(), *Inject<AssetDatabase>(), Inject<AbstractWorld>()->BuildApi(Move(api))));
	}

	// ---------------------------------------------------

	void WrenEngineComponent::WorldComponent::BindResources(JobExecutor& /*executor*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/Initialization", "Wren world boot", 0xCBABCB);

		ET_TERMINATE_WORLD_IF(_content == nullptr || _dispatcher == nullptr || _game == nullptr);
		ET_TERMINATE_WORLD_IF_FAILED(_content->BindResources());
		ET_TERMINATE_WORLD_IF_FAILED(_dispatcher->BindResources(_wren));
		ET_TERMINATE_WORLD_IF_FAILED(_game->BindResources(_wren));
	}

	// ---------------------------------------------------

	void WrenEngineComponent::WorldComponent::FreeResources(JobExecutor& executor) ETNoexceptHint {
		if (Game* const game = Exchange(_game, nullptr)) {
			game->FreeResources(_wren);
		}

		if (Dispatcher* const dispatcher = Exchange(_dispatcher, nullptr)) {
			dispatcher->FreeResources(_wren);
		}

		if (AsynchronousImport* const requiredPackages = Exchange(_worldPackages, nullptr)) {
			executor.AwaitCondition(requiredPackages->HasCompleted());
		}

		if (ContentLocator* const locator = Exchange(_content, nullptr)) {
			locator->FreeResources();
		}

		_wren.FreeResources();
	}

	// ---------------------------------------------------

	void WrenEngineComponent::WorldComponent::OnVariableRateTick(JobExecutor& /*executor*/, MicrosecondTime /*tickDuration*/, float32 /*residualFraction*/) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);

		_game->QueueEvents(*_dispatcher);
	}

	// ---------------------------------------------------

	void WrenEngineComponent::WorldComponent::OnFixedRateTick(JobExecutor& /*executor*/, MicrosecondTime delta) ETNoexceptHint {
		ET_PROFILE_SCOPE("World/Tick", "Wren script execution", 0xCBABCB);

		_dispatcher->ExecuteScriptEvents(_wren, delta);

		ET_TERMINATE_WORLD_IF(_dispatcher->ShouldShutDownWorld());
	}

}}} // namespace Eldritch2::Scripting::Wren
