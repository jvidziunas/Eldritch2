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
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxCapsuleController.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Assets;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS(CharacterPhysics) {
		api.DefineClass<CharacterPhysics>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "CharacterPhysics", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					PhysicsScene& physicsScene(GetSlotAs<PhysicsScene>(vm, 1));
					Armature&     armature(GetSlotAs<Armature>(vm, 2));

					PxCapsuleControllerDesc desc;
					desc.position = AsPxExtendedVec3(armature.GetLocalToWorld().translation);
					desc.material = ETAddressOf(physicsScene.GetCharacterMaterial());
					desc.radius   = 1.0f;
					desc.height   = 1.0f;

					PhysxPointer<PxController> controller(physicsScene.GetControllerManager().createController(desc));
					ET_ABORT_WREN_UNLESS(controller, "Error creating PhysX character controller.");
					SetReturn<CharacterPhysics>(vm, eastl::move(controller), armature);
				}), },
			{ /* Methods */
				ForeignMethod("move(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<CharacterPhysics>(vm, 0).Move(GetSlotAs<Vector>(vm, 1));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
