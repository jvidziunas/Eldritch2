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

WrenHandle* wrenGetSlotHandle( WrenVM* vm, int slot );
double		wrenGetSlotDouble( WrenVM* vm, int slot );
void		wrenSetSlotDouble( WrenVM* vm, int slot, double value );
bool		wrenGetSlotBool( WrenVM* vm, int slot );

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( Game ) {
		api.CreateClass<Game>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "Game_",
			{/* Constructors */ },
			{/*	Properties */
				DefineSetter<WrenHandle>( "whenPlayerJoins", [] ( WrenVM* vm ) {
					GetSlotAs<Game>( vm, 0 ).SetPlayerJoinHandler( vm, wrenGetSlotHandle( vm, 1 ) );
				} ),
				DefineSetter<WrenHandle>( "whenPlayerLeaves", [] ( WrenVM* vm ) {
					GetSlotAs<Game>( vm, 0 ).SetPlayerLeaveHandler( vm, wrenGetSlotHandle( vm, 1 ) );
				} ),
				DefineProperty<double>( "timeScalar",
				//	Getter
					[] ( WrenVM* vm ) {
						wrenSetSlotDouble( vm, 0, GetSlotAs<Game>( vm, 0 ).GetTimeScalar() );
					},
				//	Setter
					[] ( WrenVM* vm ) {
						GetSlotAs<Game>( vm, 0 ).SetTimeScalar( wrenGetSlotDouble( vm, 1 ) );
					}
				)
			},
			{/*	Methods */
				DefineMethod<void ( bool )>( "shutDown", [] ( WrenVM* vm ) {
					GetSlotAs<Game>( vm, 0 ).ShutDown( wrenGetSlotBool( vm, 1 ) );
				} )
			},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2