/*==================================================================*\
  EventCoordinator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/EventCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( EventCoordinator ) {
		api.CreateClass<EventCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "_EventCoordinator",
			{/* Constructors */},
			{/*	Properties */
				DefineGetter<double>( "now", [] ( WrenVM* vm ) {
					wrenSetSlotDouble( vm, 0, GetSlotAs<EventCoordinator>( vm, 0 ).GetNow() );
				} )
			},
			{/*	Methods */
				DefineMethod<void ( double, WrenHandle )>( "callAtTime", [] ( WrenVM* vm ) {
					EventCoordinator::GameTimestamp	time( AsTimestamp( wrenGetSlotDouble( vm, 1 ) ) );
					EventCoordinator&				self( GetSlotAs<EventCoordinator>( vm, 0 ) );

					ET_ABORT_WREN_IF( time <= self.GetNow(), "Events cannot be scheduled in the past!" );

					self.CallAtGameTime( time, wrenGetSlotHandle( vm, 2 ) );
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2