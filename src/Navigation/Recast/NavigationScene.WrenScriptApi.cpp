/*==================================================================*\
  NavigationScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Navigation {
namespace Recast {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS( NavigationScene ) {
		api.CreateClass<NavigationScene>( ET_BUILTIN_WREN_MODULE_NAME( Navigation ), "_NavigationScene",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Recast
}	// namespace Navigation
}	// namespace Eldritch2