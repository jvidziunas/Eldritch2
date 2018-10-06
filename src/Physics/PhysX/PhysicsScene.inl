/*==================================================================*\
  PhysicsScene.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	ETInlineHint ETForceInlineHint physx::PxControllerManager& PhysicsScene::GetControllerManager() {
		return *_controllerManager;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint physx::PxMaterial& PhysicsScene::GetCharacterMaterial() const {
		return *_knownMaterials[Character];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint physx::PxMaterial& PhysicsScene::GetTriggerMaterial() const {
		return *_knownMaterials[Trigger];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint physx::PxScene& PhysicsScene::GetScene() {
		return *_scene;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint bool PhysicsScene::IsSimulationComplete() const {
		return _scene->checkResults(/*block =*/false);
	}

	// ---------------------------------------------------

	template <typename Callback>
	ETInlineHint ETForceInlineHint bool ForEachOverlap(
		const physx::PxScene&           scene,
		const physx::PxGeometry&        geometry,
		const physx::PxTransform&       pose,
		const physx::PxVec3&            direction,
		float32                         distance,
		Callback                        callback,
		physx::PxHitFlags               hitFlags,
		const physx::PxQueryFilterData& filterData,
		physx::PxQueryFilterCallback*   filter,
		const physx::PxQueryCache*      cache,
		float32                         inflation) const {
		return scene.sweep(geometry, pose, direction, distance, callback, hitFlags, filterData, filter, cache, inflation);
	}

	// ---------------------------------------------------

	template <typename Callback>
	ETInlineHint ETForceInlineHint bool ForEachOverlap(
		const physx::PxScene&           scene,
		const physx::PxGeometry&        geometry,
		const physx::PxTransform&       pose,
		Callback                        callback,
		const physx::PxQueryFilterData& filterData,
		physx::PxQueryFilterCallback*   filter) const {
		return scene.overlap(geometry, pose, callback, filterData, filter);
	}

	// ---------------------------------------------------

	template <typename Callback>
	ETInlineHint ETForceInlineHint bool ForEachAlongRay(
		const physx::PxScene&           scene,
		const physx::PxVec3&            origin,
		const physx::PxVec3&            direction,
		float32                         distance,
		Callback                        callback,
		physx::PxHitFlags               hitFlags,
		const physx::PxQueryFilterData& filterData,
		physx::PxQueryFilterCallback*   filter,
		const physx::PxQueryCache*      cache) const {
		return scene.raycast(origin, direction, distance, callback, hitFlags, filterData, filter, cache);
	}

}}} // namespace Eldritch2::Physics::PhysX
