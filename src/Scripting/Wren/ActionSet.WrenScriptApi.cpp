/*==================================================================*\
  ActionSet.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/ActionSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( ActionSet ) {
		api.CreateClass<ActionSet>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "ActionSet",
			{/* Constructors */
				DefineConstructor<ActionSet ( WrenHandle )>( "new", [] ( WrenVM* vm ) {
					WrenHandle* const	map( wrenGetSlotHandle( vm, 1 ) );
					ActionSet&			self( SetReturn<ActionSet>( vm ) );

					self.BindResources( vm, map );

					wrenReleaseHandle( vm, map );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2