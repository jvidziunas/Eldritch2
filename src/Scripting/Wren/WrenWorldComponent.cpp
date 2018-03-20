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
#include <Scripting/Wren/EventCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/AssetDatabase.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2::Core;

	WrenWorldComponent::WrenWorldComponent(
		const World& owner
	) : WorldComponent( owner.GetServices() ),
		_wren( FindService<AssetDatabase>(), owner.GetLog() ),
		_eventCoordinator( nullptr ),
		_game( nullptr ) {
	}

// ---------------------------------------------------

	void WrenWorldComponent::AcceptVisitor( JobExecutor& /*executor*/, const EarlyInitializationVisitor ) {
		MICROPROFILE_SCOPEI( "World/EarlyInitialization", "Wren API registration", 0xCBABCB );

		_wren.BindResources( WrenHeapSizeInBytes );

	//	Build the list of application script types. This is single-threaded to avoid contention on the type and method tables.
		ApiBuilder	apiBuilder( _wren );

		for (const UniquePointer<WorldComponent>& component : FindService<World>().GetComponents()) {
			component->AcceptVisitor( apiBuilder );
		}
	}

// ---------------------------------------------------

	void WrenWorldComponent::AcceptVisitor( Scheduling::JobExecutor& /*executor*/, const LateInitializationVisitor ) {
		MICROPROFILE_SCOPEI( "World/LateInitialization", "Wren world boot", 0xCBABCB );

	/*	Import the boot script into the world scope. This will do things like configure callbacks and create the basic entity set
	 *	for gameplay. */
		if (!_wren.Interpret( R"(import "scripts/Boot")" )) {
			return FindService<World>().SetShouldShutDown();
		}
	}

// ---------------------------------------------------

	void WrenWorldComponent::AcceptVisitor( JobExecutor& /*executor*/, const StandardTickVisitor& tick ) {
		MICROPROFILE_SCOPEI( "World/Tick", "Wren script execution", 0xCBABCB );

		_eventCoordinator->ProcessTick( _wren.GetVm(), tick.durationInMicroseconds );
	}

// ---------------------------------------------------

	void WrenWorldComponent::AcceptVisitor( JobExecutor& /*executor*/, const TearDownVisitor ) {
		if (EventCoordinator* const coordinator = eastl::exchange( _eventCoordinator, nullptr ) ) {
			coordinator->FreeResources( _wren.GetVm() );
		}

		if (Game* const game = eastl::exchange( _game, nullptr )) {
			game->FreeResources( _wren.GetVm() );
		}

		_wren.FreeResources();
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2