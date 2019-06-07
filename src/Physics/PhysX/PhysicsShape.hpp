/*==================================================================*\
  PhysicsShape.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysicsShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PhysicsShape instance.
		ETConstexpr PhysicsShape(const physx::PxCapsuleGeometry&) ETNoexceptHint;
		//! Constructs this @ref PhysicsShape instance.
		ETConstexpr PhysicsShape(const physx::PxSphereGeometry&) ETNoexceptHint;
		//! Constructs this @ref PhysicsShape instance.
		ETConstexpr PhysicsShape(const physx::PxBoxGeometry&) ETNoexceptHint;
		//! Constructs this @ref PhysicsShape instance.
		ETConstexpr PhysicsShape(const PhysicsShape&) ETNoexceptHint = default;

		~PhysicsShape() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr operator const PxGeometry&() const ETNoexceptHint;

		ETConstexpr PhysicsShape& operator=(const physx::PxCapsuleGeometry&) ETNoexceptHint;
		ETConstexpr PhysicsShape& operator=(const physx::PxSphereGeometry&) ETNoexceptHint;
		ETConstexpr PhysicsShape& operator=(const physx::PxBoxGeometry&) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		union {
			physx::PxGeometry        geometry;
			physx::PxCapsuleGeometry capsule;
			physx::PxSphereGeometry  sphere;
			physx::PxBoxGeometry     box;
		} _as;
	};

}}} // namespace Eldritch2::Physics::PhysX

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysicsShape.inl>
//------------------------------------------------------------------//
