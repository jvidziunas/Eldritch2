/*==================================================================*\
  CharacterPhysics.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureAsset.hpp>
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Animation;
	using namespace ::physx;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(CharacterPhysics) {
		api.DefineClass<CharacterPhysics>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "CharacterPhysics", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					PhysicsScene& physicsScene(wrenGetSlotAs<PhysicsScene>(vm, 1));
					Armature&     armature(wrenGetSlotAs<Armature>(vm, 2));

					

					
					wrenSetReturn<CharacterPhysics>(vm, Move(controller), armature);
				}), },
			{ /* Methods */
				 }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
