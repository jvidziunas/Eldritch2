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
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE(disable : 6326)
#	include <characterkinematic/PxControllerManager.h>
#	include <characterkinematic/PxCapsuleController.h>
	ET_POP_COMPILER_WARNING_STATE()
	//------------------------------------------------------------------//

	namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {

			using namespace ::Eldritch2::Physics::PhysX::AssetViews;
			using namespace ::Eldritch2::Scripting::Wren;
			using namespace ::Eldritch2::Scripting;
			using namespace ::Eldritch2::Animation;
			using namespace ::Eldritch2::Assets;
			using namespace ::physx;

			ET_IMPLEMENT_WREN_CLASS(CharacterPhysics) {
				api.CreateClass<CharacterPhysics>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "CharacterPhysics",
												  {/* Constructors */
													  ConstructorMethod("new(_,_)", [](WrenVM* vm) {
														  PhysicsScene&	physicsScene(GetSlotAs<PhysicsScene>(vm, 1));
														  Armature&		armature(GetSlotAs<Armature>(vm, 2));

														  PxCapsuleControllerDesc desc;

														  desc.position = AsPxExtendedVec3(armature.GetLocalToWorld().translation);
														  desc.material = eastl::addressof(physicsScene.GetCharacterMaterial());
														  desc.radius = 1.0f;
														  desc.height = 1.0f;

														  PhysxPointer<PxController> controller(physicsScene.GetControllerManager().createController(desc));

														  ET_ABORT_WREN_UNLESS(controller, "Error creating PhysX character controller.");

														  SetReturn<CharacterPhysics>(vm, eastl::move(controller), armature);
													  })
												  },
													  {/*	Static methods */ },
													  {/*	Properties */ },
			{/*	Methods */
				ForeignMethod("move(_)", [](WrenVM* vm) {
					GetSlotAs<CharacterPhysics>(vm, 0).Move(GetSlotAs<Vector>(vm, 1));
				})
			}
			);
			}

		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2