/*==================================================================*\
  BipedalAgent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Navigation/Recast/NavigationScene.hpp>
#include <Navigation/Recast/BipedalAgent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Navigation {
namespace Recast {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Animation;

	ET_IMPLEMENT_WREN_CLASS( BipedalAgent ) {
		api.CreateClass<BipedalAgent>( ET_BUILTIN_WREN_MODULE_NAME( Navigation ), "BipedalAgent",
			{/* Constructors */
				DefineConstructor<BipedalAgent ( Armature )>( "new", [] ( WrenVM* vm ) {
					SetReturn<BipedalAgent>( vm, GetSlotAs<Armature>( vm, 1 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( NavigationScene, Vector )>( "navigateTo", [] ( WrenVM* vm ) {
					GetSlotAs<BipedalAgent>( vm, 0 ).BeginNavigation( GetSlotAs<NavigationScene>( vm, 1 ), GetSlotAs<Vector>( vm, 1 ) );
				} )
			},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Recast
}	// namespace Navigation
}	// namespace Eldritch2