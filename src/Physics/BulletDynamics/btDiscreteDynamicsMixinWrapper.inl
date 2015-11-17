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
#include <Utility/Math/StandardLibrary.hpp>
#include <LinearMath/btQuickProf.h>
//------------------------------------------------------------------//

template <class BasicCollisionConfiguration, class BasicConstraintSolver>
btDiscreteDynamicsMixinWrapper<BasicCollisionConfiguration, BasicConstraintSolver>::btDiscreteDynamicsMixinWrapper( ::btDispatcher* dispatcher, ::btBroadphaseInterface* pairCache,
																													ConstraintSolver* constraintSolver, CollisionConfiguration* collisionConfiguration ) : ::btDiscreteDynamicsWorld( dispatcher,
																																																									  pairCache,
																																																									  constraintSolver,
																																																									  collisionConfiguration ) {}

// ---------------------------------------------------

template <class BasicCollisionConfiguration, class BasicConstraintSolver>
int	btDiscreteDynamicsMixinWrapper<BasicCollisionConfiguration, BasicConstraintSolver>::stepSimulation2( ::btScalar timeStep, int maxSubSteps, ::btScalar fixedTimeStep ) {
#if 0
	return stepSimulation( timeStep, maxSubSteps, fixedTimeStep );
#else
	startProfiling( timeStep );

	BT_PROFILE( "stepSimulation" );

	int numSimulationSubSteps( 0 );

	if( maxSubSteps ) {
		// fixed timestep with interpolation
		m_fixedTimeStep	= fixedTimeStep;
		m_localTime		+= timeStep;

		if( m_localTime >= fixedTimeStep ) {
			numSimulationSubSteps	= static_cast<int>( m_localTime / fixedTimeStep );
			m_localTime				-= numSimulationSubSteps * fixedTimeStep;
		}
	} else {
		// variable timestep
		fixedTimeStep	= timeStep;
		m_localTime		= m_latencyMotionStateInterpolation ? 0 : timeStep;
		m_fixedTimeStep	= 0;

		if( ::btFuzzyZero( timeStep ) ) {
			numSimulationSubSteps	= 0;
			maxSubSteps				= 0;
		} else {
			numSimulationSubSteps	= 1;
			maxSubSteps				= 1;
		}
	}

	// process some debugging flags
	if( ::btIDebugDraw* const debugDrawer = getDebugDrawer() ) {
		gDisableDeactivation = (debugDrawer->getDebugMode() & btIDebugDraw::DBG_NoDeactivation) != 0;
	}

	if( numSimulationSubSteps ) {
		// clamp the number of substeps, to prevent simulation grinding spiralling down to a halt
		const int	clampedSimulationSteps( ::Eldritch2::Min( numSimulationSubSteps, maxSubSteps ) );

		saveKinematicState( fixedTimeStep * clampedSimulationSteps );
		applyGravity();

		for( int i = 0; i < clampedSimulationSteps; i++ ) {
			internalSingleStepSimulation2( fixedTimeStep );
			synchronizeMotionStates();
		}
	} else {
		synchronizeMotionStates();
	}

	clearForces();

#ifndef BT_NO_PROFILE
	CProfileManager::Increment_Frame_Counter();
#endif //BT_NO_PROFILE

	return numSimulationSubSteps;
#endif
}

// ---------------------------------------------------

template <class BasicCollisionConfiguration, class BasicConstraintSolver>
void btDiscreteDynamicsMixinWrapper<BasicCollisionConfiguration, BasicConstraintSolver>::internalSingleStepSimulation2( ::btScalar timeStep ) {
#if 0
	internalSingleStepSimulation( timeStep );
#else
	BT_PROFILE( "internalSingleStepSimulation2" );

	if( nullptr != m_internalPreTickCallback ) {
		(*m_internalPreTickCallback)( this, timeStep );
	}

	///apply gravity, predict motion
	predictUnconstraintMotion( timeStep );

	::btDispatcherInfo& dispatchInfo = getDispatchInfo();

	dispatchInfo.m_timeStep		= timeStep;
	dispatchInfo.m_stepCount	= 0;
	dispatchInfo.m_debugDraw	= getDebugDrawer();

	releasePredictiveContacts();
	createPredictiveContacts( timeStep );

	///perform collision detection
	performDiscreteCollisionDetection();
	calculateSimulationIslands();

	getSolverInfo().m_timeStep = timeStep;

	///solve contact and other joint constraints
	solveConstraints( getSolverInfo() );

	///CallbackTriggers();

	///integrate transforms

	integrateTransforms( timeStep );

	///update vehicle simulation
	updateActions( timeStep );

	updateActivationState( timeStep );

	if( 0 != m_internalTickCallback ) {
		(*m_internalTickCallback)(this, timeStep);
	}
#endif
}