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
// - TYPE PUBLISHING ---------------------------------

public:
	using CollisionConfiguration	= typename BasicCollisionConfiguration;
	using ConstraintSolver			= typename BasicConstraintSolver;

// ---

	BT_DECLARE_ALIGNED_ALLOCATOR();

// - CONSTRUCTOR/DESTRUCTOR --------------------------

	btDiscreteDynamicsMixinWrapper( ::btDispatcher* dispatcher, ::btBroadphaseInterface* pairCache, ConstraintSolver* constraintSolver, CollisionConfiguration* collisionConfiguration );

	virtual ~btDiscreteDynamicsMixinWrapper() = default;

// ---------------------------------------------------

	virtual int		stepSimulation2( ::btScalar timeStep, int maxSubSteps = 1, ::btScalar fixedTimeStep = ::btScalar(1.) / ::btScalar(60.) );

	virtual void	internalSingleStepSimulation2( ::btScalar timeStep );
};

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/BulletDynamics/btDiscreteDynamicsMixinWrapper.inl>
//------------------------------------------------------------------//