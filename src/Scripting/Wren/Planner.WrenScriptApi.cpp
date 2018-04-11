/*==================================================================*\
  Planner.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/DialogueSet.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Planner.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( Planner ) {
		api.CreateClass<Planner>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "Planner",
			{/* Constructors */
				DefineConstructor<Planner ( WrenHandle )>( "fromActions", [] ( WrenVM* vm ) {
					wrenEnsureSlots( vm, 3 );

					for (int index( wrenGetListCount( vm, 1 ) ); index != 0; --index) {
						wrenGetListElement( vm, 1, index, 2 );
					}

					SetReturn<Planner>( vm );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2