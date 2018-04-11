/*==================================================================*\
  CharacterPhysics.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureAsset.hpp>
#include <Physics/PhysX/CharacterPhysics.hpp>
#include <Scripting/Wren/AssetReference.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <characterkinematic/PxControllerManager.h>
ET_POP_COMPILER_WARNING_STATE()
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Animation;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS( CharacterPhysics ) {
		api.CreateClass<CharacterPhysics>( ET_BUILTIN_WREN_MODULE_NAME( Physics ), "CharacterPhysics",
			{/* Constructors */
				DefineConstructor<CharacterPhysics ( PhysicsScene, Armature, AssetReference<ArmatureAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<CharacterPhysics>( vm, nullptr, GetSlotAs<Armature>( vm, 2 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ( Vector )>( "move", [] ( WrenVM* vm ) {
					GetSlotAs<CharacterPhysics>( vm, 0 ).Move( GetSlotAs<Vector>( vm, 1 ) );
				} )
			},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2