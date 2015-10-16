/*==================================================================*\
  btSoftBodyWorldMixin.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <BulletSoftBody/btSoftBody.h>
//------------------------------------------------------------------//

class	btSoftBodySolver;

template <typename BaseType>
class btSoftBodyWorldMixin : public BaseType {
public:
	class CollisionConfiguration : public BaseType::CollisionConfiguration {
	public:
		typedef typename BaseType::CollisionConfiguration	UnderlyingConfiguration;

	// ---------------------------------------------------

		template <typename... ConstructorArguments>
		CollisionConfiguration( ConstructorArguments&&... constructorArguments );

		virtual ~CollisionConfiguration();

	// ---------------------------------------------------

		///creation of soft-soft and soft-rigid, and otherwise fallback to base class implementation
		virtual btCollisionAlgorithmCreateFunc* getCollisionAlgorithmCreateFunc( int proxyType0, int proxyType1 ) override;

	// ---------------------------------------------------

		::btCollisionAlgorithmCreateFunc*	m_softSoftCreateFunc;
		::btCollisionAlgorithmCreateFunc*	m_softRigidConvexCreateFunc;
		::btCollisionAlgorithmCreateFunc*	m_swappedSoftRigidConvexCreateFunc;
		::btCollisionAlgorithmCreateFunc*	m_softRigidConcaveCreateFunc;
		::btCollisionAlgorithmCreateFunc*	m_swappedSoftRigidConcaveCreateFunc;
	};

// ---

	typedef typename BaseType::ConstraintSolver	ConstraintSolver;

// ---------------------------------------------------

public:
	// Constructs this btSoftBodyWorldMixin.
	template <typename... ConstructorArguments>
	btSoftBodyWorldMixin( ::btSoftBodySolver* softBodySolver, ConstructorArguments&&... constructorArguments );

	// Destroys this btSoftBodyWorldMixin.
	virtual ~btSoftBodyWorldMixin();

// ---------------------------------------------------

	void	debugDrawWorld() override;

// ---------------------------------------------------

	void	addSoftBody( ::btSoftBody* body, short int collisionFilterGroup = ::btBroadphaseProxy::DefaultFilter, short int collisionFilterMask = ::btBroadphaseProxy::AllFilter );

	void	removeSoftBody( ::btSoftBody* body );

	///removeCollisionObject will first check if it is a rigid body, if so call removeRigidBody otherwise call btDiscreteDynamicsWorld::removeCollisionObject
	void	removeCollisionObject( ::btCollisionObject* collisionObject ) override;

// ---------------------------------------------------

	int		getDrawFlags() const;

	void	setDrawFlags( int f );

// ---------------------------------------------------

	::btSoftBodyWorldInfo&			getWorldInfo();
	const ::btSoftBodyWorldInfo&	getWorldInfo() const;

// ---------------------------------------------------

	::btDynamicsWorldType	getWorldType() const override;

// ---------------------------------------------------

	::btAlignedObjectArray<::btSoftBody*>&			getSoftBodyArray();
	const ::btAlignedObjectArray<::btSoftBody*>&	getSoftBodyArray() const;

// ---------------------------------------------------

	void	rayTest( const ::btVector3& rayFromWorld, const ::btVector3& rayToWorld, ::btCollisionWorld::RayResultCallback& resultCallback ) const override;

	/// rayTestSingle performs a raycast call and calls the resultCallback. It is used internally by rayTest.
	/// In a future implementation, we consider moving the ray test as a virtual method in btCollisionShape.
	/// This allows more customization.
	static void	rayTestSingle( const ::btTransform& rayFromTrans, const ::btTransform& rayToTrans, ::btCollisionObject* collisionObject, const ::btCollisionShape* collisionShape, const ::btTransform& colObjWorldTransform, ::btCollisionWorld::RayResultCallback& resultCallback );

// ---------------------------------------------------

	void	serialize( ::btSerializer* serializer ) override;

// ---------------------------------------------------

protected:
	void	predictUnconstraintMotion( ::btScalar timeStep ) override;

	void	internalSingleStepSimulation( ::btScalar timeStep ) override;

	void	solveSoftBodiesConstraints( ::btScalar timeStep );

	void	serializeSoftBodies( ::btSerializer* serializer );

// ---------------------------------------------------

private:
	::btAlignedObjectArray<::btSoftBody*>	m_softBodies;
	int										m_drawFlags;
	bool									m_drawNodeTree;
	bool									m_drawFaceTree;
	bool									m_drawClusterTree;
	::btSoftBodyWorldInfo					m_sbi;
	///Solver classes that encapsulate multiple soft bodies for solving
	::btSoftBodySolver*						m_softBodySolver;
	bool									m_ownsSolver;
};

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/Bullet/btSoftBodyWorldMixin.inl>
//------------------------------------------------------------------//