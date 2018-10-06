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
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Assets/ContentLocator.hpp>
#include <Physics/PhysX/Physics.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <PxPhysics.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Assets;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS(Physics) {
		api.DefineClass<Physics>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "Physics", // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					const PhysicsAsset*       asset(Cast<PhysicsAsset>(GetSlotAs<AssetReference>(vm, 2)));
					PhysxPointer<PxAggregate> aggregate(PxGetPhysics().createAggregate(1u, /*enableSelfCollision =*/false));
					ET_ABORT_WREN_UNLESS(aggregate, "Error creating PhysX aggregate.");

					for (const PhysicsAsset::RigidShape& shape : asset->GetRigidShapes()) {

					}

					SetReturn<Physics>(vm, /*classSlot =*/0, *asset, GetSlotAs<Armature>(vm, 1).InsertClip<PhysicsClip>(eastl::move(aggregate)));
				}), },
			{/*	Methods */ }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
