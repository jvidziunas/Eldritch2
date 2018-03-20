/*==================================================================*\
  Physics.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureAsset.hpp>
#include <Scripting/Wren/AssetReference.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Physics/PhysX/Physics.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
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

	ET_IMPLEMENT_WREN_CLASS( Physics ) {
		api.CreateClass<Physics>( ET_BUILTIN_WREN_MODULE_NAME( Physics ), "Physics",
			{/* Constructors */
				DefineConstructor<Physics ( PhysicsScene, Armature, AssetReference<ArmatureAsset> )>( "new", [] ( WrenVM* vm ) {
					SetReturn<Physics>(
						vm,
						PhysxPointer<PxArticulation>( nullptr ),
						GetSlotAs<Armature>( vm, 2 ),
						GetSlotAs<AssetReference<ArmatureAsset>>( vm, 3 )
					);
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2