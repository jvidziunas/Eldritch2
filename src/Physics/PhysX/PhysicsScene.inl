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

	template <typename Callback>
	ETInlineHint bool PhysicsScene::SweepGeometry(
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
		return _scene->sweep(geometry, pose, direction, distance, callback, hitFlags, filterData, filter, cache, inflation);
	}

	// ---------------------------------------------------

	template <typename Callback>
	ETInlineHint bool PhysicsScene::CheckOverlap(
		const physx::PxGeometry&        geometry,
		const physx::PxTransform&       pose,
		Callback                        callback,
		const physx::PxQueryFilterData& filterData,
		physx::PxQueryFilterCallback*   filter) const {
		return _scene->overlap(geometry, pose, callback, filterData, filter);
	}

	// ---------------------------------------------------

	template <typename Callback>
	ETInlineHint bool PhysicsScene::SweepRay(
		const physx::PxVec3&            origin,
		const physx::PxVec3&            direction,
		float32                         distance,
		Callback                        callback,
		physx::PxHitFlags               hitFlags,
		const physx::PxQueryFilterData& filterData,
		physx::PxQueryFilterCallback*   filter,
		const physx::PxQueryCache*      cache) const {
		return _scene->raycast(origin, direction, distance, callback, hitFlags, filterData, filter, cache);
	}

	// ---------------------------------------------------

	ETInlineHint physx::PxControllerManager& PhysicsScene::GetControllerManager() {
		return *_controllerManager;
	}

	// ---------------------------------------------------

	ETInlineHint physx::PxMaterial& PhysicsScene::GetCharacterMaterial() const {
		return *_characterMaterial;
	}

	// ---------------------------------------------------

	ETInlineHint physx::PxMaterial& PhysicsScene::GetTriggerMaterial() const {
		return *_triggerMaterial;
	}

	// ---------------------------------------------------

	ETInlineHint physx::PxScene& PhysicsScene::GetScene() {
		return *_scene;
	}

	// ---------------------------------------------------

	ETInlineHint bool PhysicsScene::IsSimulationComplete() const {
		return _scene->checkResults();
	}

}}} // namespace Eldritch2::Physics::PhysX
