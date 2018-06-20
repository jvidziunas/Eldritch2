/*==================================================================*\
  TerrainPhysics.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxPointer.hpp>
//------------------------------------------------------------------//

namespace physx {
class PxRigidStatic;
class PxScene;
} // namespace physx

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class TerrainPhysics {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref TerrainPhysics instance.
		TerrainPhysics(PhysxPointer<physx::PxRigidStatic> actor);
		//!	Disable copy construction.
		TerrainPhysics(const TerrainPhysics&) = delete;

		~TerrainPhysics() = default;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TerrainPhysics& operator=(const TerrainPhysics&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const PhysxPointer<physx::PxRigidStatic> _actor;
	};

}}} // namespace Eldritch2::Physics::PhysX
