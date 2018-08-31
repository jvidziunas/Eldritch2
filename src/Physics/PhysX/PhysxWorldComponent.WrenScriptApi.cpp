/*==================================================================*\
  PhysxWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxWorldComponent.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxControllerManager.h>
#include <PxSceneDesc.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {
	ET_DECLARE_WREN_CLASS(CharacterPhysics);
	ET_DECLARE_WREN_CLASS(TerrainPhysics);
	ET_DECLARE_WREN_CLASS(PhysicsShape);
	ET_DECLARE_WREN_CLASS(PhysicsScene);
	ET_DECLARE_WREN_CLASS(TriggerArea);
	ET_DECLARE_WREN_CLASS(Physics);
}}} // namespace Eldritch2::Physics::PhysX

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	void PhysxWorldComponent::DefineScriptApi(ApiBuilder& api) {
		PxSceneDesc sceneProperties(PxGetPhysics().getTolerancesScale());

		sceneProperties.filterShader  = ETAddressOf(PhysicsScene::FilterShader);
		sceneProperties.cpuDispatcher = this;

		//	Create the core PhysX scene object, responsible for simulation and communication across the various actors, etc. in the physics scene.
		PhysxPointer<PxScene> scene(PxGetPhysics().createScene(sceneProperties));
		if (!scene) {
			FindService<World>()->SetShouldShutDown();
			return;
		}

		PhysxPointer<PxControllerManager> controllerManager(PxCreateControllerManager(*scene));
		if (!controllerManager) {
			FindService<World>()->SetShouldShutDown();
			return;
		}

		PhysxPointer<PxMaterial> characterMaterial(MakeCharacterMaterial());
		if (!characterMaterial) {
			FindService<World>()->SetShouldShutDown();
			return;
		}

		PhysxPointer<PxMaterial> triggerMaterial(MakeTriggerMaterial());
		if (!triggerMaterial) {
			FindService<World>()->SetShouldShutDown();
			return;
		}

		ET_REGISTER_WREN_CLASS(CharacterPhysics, api);
		ET_REGISTER_WREN_CLASS(TerrainPhysics, api);
		ET_REGISTER_WREN_CLASS(PhysicsShape, api);
		ET_REGISTER_WREN_CLASS(PhysicsScene, api);
		ET_REGISTER_WREN_CLASS(TriggerArea, api);
		ET_REGISTER_WREN_CLASS(Physics, api);

		_scene = api.CreateVariable<PhysicsScene>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "PhysicsScene", eastl::move(scene), eastl::move(controllerManager), eastl::move(characterMaterial), eastl::move(triggerMaterial));
	}

}}} // namespace Eldritch2::Physics::PhysX
