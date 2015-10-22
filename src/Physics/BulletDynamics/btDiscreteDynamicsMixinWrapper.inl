/*==================================================================*\
  btDiscreteDynamicsMixinWrapper.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

template <class BasicCollisionConfiguration, class BasicConstraintSolver>
btDiscreteDynamicsMixinWrapper<BasicCollisionConfiguration, BasicConstraintSolver>::btDiscreteDynamicsMixinWrapper( ::btDispatcher* dispatcher, ::btBroadphaseInterface* pairCache, ConstraintSolver* constraintSolver, CollisionConfiguration* collisionConfiguration ) : ::btDiscreteDynamicsWorld( dispatcher, pairCache, constraintSolver, collisionConfiguration ) {}