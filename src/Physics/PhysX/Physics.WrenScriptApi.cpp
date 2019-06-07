/*==================================================================*\
  Physics.WrenScriptApi.cpp
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
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Physics/PhysX/Physics.hpp>
#include <Core/ContentLocator.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxCapsuleController.h>
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(Physics, api, moduleName) {
		api.DefineClass<Physics>("Physics", moduleName, // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					PhysicsScene& physicsScene(wrenGetSlotAs<PhysicsScene>(vm, 1));
					Armature&     armature(wrenGetSlotAs<Armature>(vm, 2));

					const PhysicsAsset* asset(Get<PhysicsAsset>(wrenGetSlotAs<AssetReference>(vm, 3)));
					ET_ABORT_WREN_IF(asset == nullptr, vm, "Asset must be a PhysicsAsset.");

					PxCapsuleControllerDesc desc;
					desc.position = AsPxExtendedVec3(armature.GetLocalToWorld().translation);
					desc.material = ETAddressOf(physicsScene.GetMaterial(Character));
					desc.radius   = 1.0f;
					desc.height   = 1.0f;

					PhysxPointer<PxController> controller(physicsScene.GetControllerManager().createController(desc));
					ET_ABORT_WREN_IF(controller == nullptr, vm, "Error creating PhysX character controller.");
					
					PhysxPointer<PxAggregate> aggregate(PxGetPhysics().createAggregate(/*maxSize =*/1u, /*enableSelfCollision =*/false));
					ET_ABORT_WREN_IF(aggregate == nullptr, vm, "Error creating PhysX aggregate.");

					for (const PhysicsAsset::RigidShape& shape : asset->GetRigidShapes()) {
						// Add shapes to aggregate
					}

					wrenSetReturn<Physics>(vm, /*classSlot =*/0, *asset, Move(controller), armature.InsertClip<PhysicsClip>(Move(aggregate)));
				}), },
			{/*	Methods */
				ForeignMethod("moveTarget=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Physics>(vm, 0).SetMovement(wrenGetSlotAs<Vector>(vm, 1));
				}),
			}); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
