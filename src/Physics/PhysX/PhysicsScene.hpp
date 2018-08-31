/*==================================================================*\
  PhysicsScene.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXPointer.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxControllerManager.h>
#include <PxContactModifyCallback.h>
#include <PxQueryReport.h>
#include <PxFiltering.h>
#include <PxMaterial.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysicsScene : public physx::PxContactModifyCallback {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PhysicsScene instance.
		PhysicsScene(
			PhysxPointer<physx::PxScene>             scene,
			PhysxPointer<physx::PxControllerManager> controllerManager,
			PhysxPointer<physx::PxMaterial>          characterMaterial,
			PhysxPointer<physx::PxMaterial>          triggerMaterial);
		//! Disable copy construction.
		PhysicsScene(const PhysicsScene&) = delete;

		~PhysicsScene() = default;

		// ---------------------------------------------------

	public:
		physx::PxControllerManager& GetControllerManager();

		physx::PxScene& GetScene();

		physx::PxMaterial& GetCharacterMaterial() const;
		physx::PxMaterial& GetTriggerMaterial() const;

		// ---------------------------------------------------

	public:
		bool IsSimulationComplete() const;

		// ---------------------------------------------------

	public:
		ErrorCode StartSimulation(physx::PxReal duration);

		ErrorCode FinishSimulation();

		// - PXCONTACTMODIFYCALLBACK METHODS -----------------

	private:
		void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override;

		// ---------------------------------------------------

	public:
		static physx::PxFilterFlags FilterShader(
			physx::PxFilterObjectAttributes lhsAttributes,
			physx::PxFilterData             lhsFilter,
			physx::PxFilterObjectAttributes rhsAttributes,
			physx::PxFilterData             rhsFilter,
			physx::PxPairFlags&             flags,
			const void*                     constants,
			physx::PxU32                    constantsSize);

		// - DATA MEMBERS ------------------------------------

	private:
		PhysxPointer<physx::PxScene>             _scene;
		PhysxPointer<physx::PxControllerManager> _controllerManager;
		PhysxPointer<physx::PxMaterial>          _characterMaterial;
		PhysxPointer<physx::PxMaterial>          _triggerMaterial;
		ET16ByteAligned char                     _scratchMemory[16384u /* 16KB*/];
	};

	// ---

	PhysxPointer<physx::PxMaterial> MakeCharacterMaterial();
	PhysxPointer<physx::PxMaterial> MakeTriggerMaterial();

	// ---

	template <typename Callback>
	bool ForEachOverlap(
		const physx::PxScene&           scene,
		const physx::PxGeometry&        geometry,
		const physx::PxTransform&       pose,
		const physx::PxVec3&            direction,
		float32                         distance,
		Callback                        callback,
		physx::PxHitFlags               hitFlags   = physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
		const physx::PxQueryFilterData& filterData = physx::PxQueryFilterData(),
		physx::PxQueryFilterCallback*   filter     = nullptr,
		const physx::PxQueryCache*      cache      = nullptr,
		float32                         inflation  = 0.0f);

	template <typename Callback>
	bool ForEachOverlap(
		const physx::PxScene&           scene,
		const physx::PxGeometry&        geometry,
		const physx::PxTransform&       pose,
		Callback                        callback,
		const physx::PxQueryFilterData& filterData = physx::PxQueryFilterData(),
		physx::PxQueryFilterCallback*   filter     = nullptr);

	template <typename Callback>
	bool ForEachAlongRay(
		const physx::PxScene&           scene,
		const physx::PxVec3&            origin,
		const physx::PxVec3&            direction,
		float32                         distance,
		Callback                        callback,
		physx::PxHitFlags               hitFlags   = physx::PxHitFlags(physx::PxHitFlag::eDEFAULT),
		const physx::PxQueryFilterData& filterData = physx::PxQueryFilterData(),
		physx::PxQueryFilterCallback*   filter     = nullptr,
		const physx::PxQueryCache*      cache      = nullptr);

}}} // namespace Eldritch2::Physics::PhysX

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysicsScene.inl>
//------------------------------------------------------------------//
