/*==================================================================*\
  Game.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Game.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( Game ) {
		api.CreateClass<Game>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "_Game",
			{/* Constructors */ },
			{/*	Properties */
				DefineSetter<WrenHandle>( "whenPlayerJoins", [] ( WrenVM* vm ) {
					GetSlotAs<Game>( vm, 0 ).SetPlayerJoinHandler( vm, wrenGetSlotHandle( vm, 1 ) );
				} ),
				DefineSetter<WrenHandle>( "whenPlayerLeaves", [] ( WrenVM* vm ) {
					GetSlotAs<Game>( vm, 0 ).SetPlayerLeaveHandler( vm, wrenGetSlotHandle( vm, 1 ) );
				} )
			},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2