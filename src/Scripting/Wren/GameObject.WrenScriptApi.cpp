/*==================================================================*\
  GameObject.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/GameObject.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ET_IMPLEMENT_WREN_CLASS( GameObject ) {
		api.CreateClass<GameObject>( ET_BUILTIN_WREN_MODULE_NAME( Core ), "GameObject",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2