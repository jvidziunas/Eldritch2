/*==================================================================*\
  btDiscreteDynamicsMixinWrapper.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
//------------------------------------------------------------------//

template <class BasicCollisionConfiguration = ::btDefaultCollisionConfiguration, class BasicConstraintSolver = ::btSequentialImpulseConstraintSolver>
class btDiscreteDynamicsMixinWrapper : public ::btDiscreteDynamicsWorld {
public:
	typedef typename BasicCollisionConfiguration	CollisionConfiguration;
	typedef typename BasicConstraintSolver			ConstraintSolver;

// ---

	BT_DECLARE_ALIGNED_ALLOCATOR();

	btDiscreteDynamicsMixinWrapper( btDispatcher* dispatcher, btBroadphaseInterface* pairCache, ConstraintSolver* constraintSolver, CollisionConfiguration* collisionConfiguration );

	virtual ~btDiscreteDynamicsMixinWrapper() = default;
};

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/BulletDynamics/btDiscreteDynamicsMixinWrapper.inl>
//------------------------------------------------------------------//