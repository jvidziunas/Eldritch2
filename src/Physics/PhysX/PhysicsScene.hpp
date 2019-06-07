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
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <characterkinematic/PxControllerManager.h>
#include <PxContactModifyCallback.h>
#include <PxQueryReport.h>
#include <PxMaterial.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	enum KnownMaterial {
		Trigger,
		Character,

		KnownMaterials
	};

	// ---

	class PhysicsScene : public physx::PxContactModifyCallback {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		PhysicsScene(const PhysicsScene&) = delete;
		//! Constructs this @ref PhysicsScene instance.
		PhysicsScene() ETNoexceptHint;

		~PhysicsScene() = default;

		// ---------------------------------------------------

	public:
		physx::PxControllerManager& GetControllerManager();

		physx::PxScene& GetScene();

		ETConstexpr physx::PxMaterial& GetMaterial(KnownMaterial material) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool IsSimulationComplete() const;

		// ---------------------------------------------------

	public:
		void Simulate(MicrosecondTime duration);

		Result FinishSimulation();

		// ---------------------------------------------------

	public:
		Result BindResources(physx::PxCpuDispatcher& dispatcher);

		void FreeResources();

		// - PXCONTACTMODIFYCALLBACK METHODS -----------------

	private:
		void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) override;

		// - DATA MEMBERS ------------------------------------

	private:
		PhysxPointer<physx::PxScene>             _scene;
		PhysxPointer<physx::PxControllerManager> _controllerManager;
		PhysxPointer<physx::PxMaterial>          _knownMaterials[KnownMaterials];
		ET16ByteAligned char                     _scratchMemory[16384];
	};

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
