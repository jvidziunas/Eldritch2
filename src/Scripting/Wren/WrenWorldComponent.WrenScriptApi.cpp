/*==================================================================*\
  WrenWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/WrenWorldComponent.hpp>
#include <Scripting/Wren/EventCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Game.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {
			ET_DECLARE_WREN_CLASS( EventCoordinator );
			ET_DECLARE_WREN_CLASS( DialogueSet );
			ET_DECLARE_WREN_CLASS( GameObject );
			ET_DECLARE_WREN_CLASS( ActionSet );
			ET_DECLARE_WREN_CLASS( Planner );
			ET_DECLARE_WREN_CLASS( Game );

			ET_DECLARE_WREN_CLASS( Transformation );
			ET_DECLARE_WREN_CLASS( Quaternion );
			ET_DECLARE_WREN_CLASS( Vector );
		}
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	using namespace ::Eldritch2::Core;

	void WrenWorldComponent::AcceptVisitor( ApiBuilder& api ) {
		ET_REGISTER_WREN_CLASS( Transformation, api );
		ET_REGISTER_WREN_CLASS( Quaternion, api );
		ET_REGISTER_WREN_CLASS( Vector, api );

		ET_REGISTER_WREN_CLASS( EventCoordinator, api );
		ET_REGISTER_WREN_CLASS( DialogueSet, api );
		ET_REGISTER_WREN_CLASS( GameObject, api );
		ET_REGISTER_WREN_CLASS( ActionSet, api );
		ET_REGISTER_WREN_CLASS( Planner, api );
		ET_REGISTER_WREN_CLASS( Game, api );

		_game				= api.CreateVariable<Game>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "Game", FindService<World>() );
		_eventCoordinator	= api.CreateVariable<EventCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "EventCoordinator" );
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2