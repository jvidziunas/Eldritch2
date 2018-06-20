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
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Physics/PhysX/Physics.hpp>
#include <Assets/AssetManager.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <PxPhysics.h>
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

			ET_IMPLEMENT_WREN_CLASS(Physics) {
				api.CreateClass<Physics>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "Physics",
										 {/* Constructors */
											 ConstructorMethod("new(_,_)", [](WrenVM* vm) {
												 PhysicsScene&	physicsScene(GetSlotAs<PhysicsScene>(vm, 1));
												 Armature&		armature(GetSlotAs<Armature>(vm, 2));

												 PhysxPointer<PxArticulation>	articulation(PxGetPhysics().createArticulation());
												 ET_ABORT_WREN_UNLESS(articulation, "Error creating PhysX articulation.");

												 armature.AddClip<Physics::AnimationClip>(*articulation);

												 physicsScene.GetScene().addArticulation(*articulation);
												 SetReturn<Physics>(vm, eastl::move(articulation), armature);
											 })
										 },
											 {/*	Static methods */ },
											 {/*	Properties */ },
											 {/*	Methods */ }
											 );
			}

		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2