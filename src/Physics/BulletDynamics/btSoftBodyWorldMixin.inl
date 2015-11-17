/*==================================================================*\
  btSoftBodyWorldMixin.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h>
#include <BulletSoftBody/btSoftRigidCollisionAlgorithm.h>
#include <BulletSoftBody/btSoftSoftCollisionAlgorithm.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <LinearMath/btPoolAllocator.h>
#include <LinearMath/btSerializer.h>
#include <LinearMath/btQuickprof.h>
#include <type_traits>
//------------------------------------------------------------------//

#define ENABLE_SOFTBODY_CONCAVE_COLLISIONS 1

template <typename BaseType>
template <typename... ConstructorArguments>
btSoftBodyWorldMixin<BaseType>::CollisionConfiguration::CollisionConfiguration( ConstructorArguments&&... constructorArguments ) : UnderlyingConfiguration( ::std::forward<ConstructorArguments>(constructorArguments)... ) {
	void* mem;

	mem = btAlignedAlloc( sizeof(::btSoftSoftCollisionAlgorithm::CreateFunc), 16 );
	m_softSoftCreateFunc = new(mem) btSoftSoftCollisionAlgorithm::CreateFunc;

	mem = btAlignedAlloc( sizeof(::btSoftRigidCollisionAlgorithm::CreateFunc), 16 );
	m_softRigidConvexCreateFunc = new(mem) btSoftRigidCollisionAlgorithm::CreateFunc;

	mem = btAlignedAlloc( sizeof(::btSoftRigidCollisionAlgorithm::CreateFunc), 16 );
	m_swappedSoftRigidConvexCreateFunc = new(mem) btSoftRigidCollisionAlgorithm::CreateFunc;
	m_swappedSoftRigidConvexCreateFunc->m_swapped = true;

#ifdef ENABLE_SOFTBODY_CONCAVE_COLLISIONS
	mem = btAlignedAlloc( sizeof(::btSoftBodyConcaveCollisionAlgorithm::CreateFunc), 16 );
	m_softRigidConcaveCreateFunc = new(mem) btSoftBodyConcaveCollisionAlgorithm::CreateFunc;

	mem = btAlignedAlloc( sizeof(::btSoftBodyConcaveCollisionAlgorithm::CreateFunc), 16 );
	m_swappedSoftRigidConcaveCreateFunc = new(mem) btSoftBodyConcaveCollisionAlgorithm::SwappedCreateFunc;
	m_swappedSoftRigidConcaveCreateFunc->m_swapped = true;
#endif

	//replace pool by a new one, with potential larger size

	if( m_ownsCollisionAlgorithmPool && m_collisionAlgorithmPool ) {
		///calculate maximum element size, big enough to fit any collision algorithm in the memory pool
		int	collisionAlgorithmMaxElementSize( ::btMax( ::btMax( sizeof(::btSoftSoftCollisionAlgorithm), sizeof(::btSoftRigidCollisionAlgorithm) ), sizeof(::btSoftBodyConcaveCollisionAlgorithm) ) );
		int curElemSize( m_collisionAlgorithmPool->getElementSize() );
		int maxPoolSize( m_collisionAlgorithmPool->getMaxCount() );

		if( collisionAlgorithmMaxElementSize > curElemSize ) {
			btAlignedFree( (m_collisionAlgorithmPool->~btPoolAllocator(), m_collisionAlgorithmPool) );
			m_collisionAlgorithmPool = new(btAlignedAlloc( sizeof(::btPoolAllocator), 16 )) ::btPoolAllocator( collisionAlgorithmMaxElementSize, maxPoolSize );
		}
	}
}

// ---------------------------------------------------

template <typename BaseType>
btSoftBodyWorldMixin<BaseType>::CollisionConfiguration::~CollisionConfiguration() {
	m_softSoftCreateFunc->~btCollisionAlgorithmCreateFunc();
	btAlignedFree( m_softSoftCreateFunc );

	m_softRigidConvexCreateFunc->~btCollisionAlgorithmCreateFunc();
	btAlignedFree( m_softRigidConvexCreateFunc );

	m_swappedSoftRigidConvexCreateFunc->~btCollisionAlgorithmCreateFunc();
	btAlignedFree( m_swappedSoftRigidConvexCreateFunc );

#ifdef ENABLE_SOFTBODY_CONCAVE_COLLISIONS
	m_softRigidConcaveCreateFunc->~btCollisionAlgorithmCreateFunc();
	btAlignedFree( m_softRigidConcaveCreateFunc );

	m_swappedSoftRigidConcaveCreateFunc->~btCollisionAlgorithmCreateFunc();
	btAlignedFree( m_swappedSoftRigidConcaveCreateFunc );
#endif
}

// ---------------------------------------------------

template <typename BaseType>
///creation of soft-soft and soft-rigid, and otherwise fallback to base class implementation
::btCollisionAlgorithmCreateFunc* btSoftBodyWorldMixin<BaseType>::CollisionConfiguration::getCollisionAlgorithmCreateFunc( int proxyType0, int proxyType1 ) {

	///try to handle the softbody interactions first

	if( (proxyType0 == SOFTBODY_SHAPE_PROXYTYPE) && (proxyType1 == SOFTBODY_SHAPE_PROXYTYPE) ) {
		return	m_softSoftCreateFunc;
	}

	///softbody versus convex
	if( proxyType0 == SOFTBODY_SHAPE_PROXYTYPE  && btBroadphaseProxy::isConvex( proxyType1 ) ) {
		return	m_softRigidConvexCreateFunc;
	}

	///convex versus soft body
	if( btBroadphaseProxy::isConvex( proxyType0 ) && proxyType1 == SOFTBODY_SHAPE_PROXYTYPE ) {
		return	m_swappedSoftRigidConvexCreateFunc;
	}

#ifdef ENABLE_SOFTBODY_CONCAVE_COLLISIONS
	///softbody versus convex
	if( proxyType0 == SOFTBODY_SHAPE_PROXYTYPE  && btBroadphaseProxy::isConcave( proxyType1 ) ) {
		return	m_softRigidConcaveCreateFunc;
	}

	///convex versus soft body
	if( btBroadphaseProxy::isConcave( proxyType0 ) && proxyType1 == SOFTBODY_SHAPE_PROXYTYPE ) {
		return	m_swappedSoftRigidConcaveCreateFunc;
	}
#endif

	///fallback to the regular rigid collision shape
	return UnderlyingConfiguration::getCollisionAlgorithmCreateFunc( proxyType0, proxyType1 );
}

// ---------------------------------------------------

template <typename BaseType>
template <typename... ConstructorArguments>
btSoftBodyWorldMixin<BaseType>::btSoftBodyWorldMixin( ::btSoftBodySolver* softBodySolver, ConstructorArguments&&... constructorArguments ) : BaseType( ::std::forward<ConstructorArguments>(constructorArguments)... ),
																																			 m_softBodySolver( softBodySolver ),
																																			 m_ownsSolver(false) {
	if( !m_softBodySolver ) {
		void* ptr = btAlignedAlloc( sizeof(::btDefaultSoftBodySolver), 16 );
		m_softBodySolver = new(ptr) ::btDefaultSoftBodySolver();
		m_ownsSolver = true;
	}

	m_drawFlags = fDrawFlags::Std;
	m_drawNodeTree = true;
	m_drawFaceTree = false;
	m_drawClusterTree = false;
	m_sbi.m_broadphase = getBroadphase();
	m_sbi.m_dispatcher = getDispatcher();
	m_sbi.m_sparsesdf.Initialize();
	m_sbi.m_sparsesdf.Reset();

	m_sbi.air_density = ::btScalar( 1.2 );
	m_sbi.water_density = 0;
	m_sbi.water_offset = 0;
	m_sbi.water_normal = btVector3( 0, 0, 0 );
	m_sbi.m_gravity = getGravity();

	m_sbi.m_sparsesdf.Initialize();
}

// ---------------------------------------------------

template <typename BaseType>
btSoftBodyWorldMixin<BaseType>::~btSoftBodyWorldMixin() {
	if( m_ownsSolver ) {
		m_softBodySolver->~btSoftBodySolver();
		btAlignedFree( m_softBodySolver );
	}
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::predictUnconstraintMotion( ::btScalar timeStep ) {
	BaseType::predictUnconstraintMotion( timeStep );

	{	BT_PROFILE( "predictUnconstraintMotionSoftBody" );
		m_softBodySolver->predictMotion( float( timeStep ) );
	}
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::internalSingleStepSimulation( ::btScalar timeStep ) {
	// Let the solver grab the soft bodies and if necessary optimize for it
	m_softBodySolver->optimize( getSoftBodyArray() );

	if( !m_softBodySolver->checkInitialized() ) {
		btAssert( "Solver initialization failed\n" );
	}

	BaseType::internalSingleStepSimulation( timeStep );

	///solve soft bodies constraints
	solveSoftBodiesConstraints( timeStep );

	//self collisions
	for( int i = 0; i < m_softBodies.size(); i++ ) {
		::btSoftBody*	psb = static_cast<::btSoftBody*>(m_softBodies[i]);
		psb->defaultCollisionHandler( psb );
	}

	///update soft bodies
	m_softBodySolver->updateSoftBodies();

	// End solver-wise simulation step
	// ///////////////////////////////
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::solveSoftBodiesConstraints( ::btScalar timeStep ) {
	BT_PROFILE( "solveSoftConstraints" );

	if( m_softBodies.size() ) {
		btSoftBody::solveClusters( m_softBodies );
	}

	// Solve constraints solver-wise
	m_softBodySolver->solveConstraints( timeStep * m_softBodySolver->getTimeScale() );
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::serializeSoftBodies( ::btSerializer* serializer ) {
	int i;
	//serialize all collision objects
	for( i = 0; i < m_collisionObjects.size(); i++ ) {
		::btCollisionObject*	colObj = m_collisionObjects[i];
		if( colObj->getInternalType() & btCollisionObject::CO_SOFT_BODY ) {
			int len = colObj->calculateSerializeBufferSize();
			btChunk* chunk = serializer->allocate( len, 1 );
			const char* structType = colObj->serialize( chunk->m_oldPtr, serializer );
			serializer->finalizeChunk( chunk, structType, BT_SOFTBODY_CODE, colObj );
		}
	}
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::debugDrawWorld() {
	BaseType::debugDrawWorld();

	if( getDebugDrawer() ) {
		int i;
		for( i = 0; i < this->m_softBodies.size(); i++ ) {
			::btSoftBody*	psb = static_cast<btSoftBody*>(this->m_softBodies[i]);
			if( getDebugDrawer() && (getDebugDrawer()->getDebugMode() & (btIDebugDraw::DBG_DrawWireframe)) ) {
				btSoftBodyHelpers::DrawFrame( psb, m_debugDrawer );
				btSoftBodyHelpers::Draw( psb, m_debugDrawer, m_drawFlags );
			}

			if( m_debugDrawer && (m_debugDrawer->getDebugMode() & btIDebugDraw::DBG_DrawAabb) ) {
				if( m_drawNodeTree )	btSoftBodyHelpers::DrawNodeTree( psb, m_debugDrawer );
				if( m_drawFaceTree )	btSoftBodyHelpers::DrawFaceTree( psb, m_debugDrawer );
				if( m_drawClusterTree )	btSoftBodyHelpers::DrawClusterTree( psb, m_debugDrawer );
			}
		}
	}
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::addSoftBody( ::btSoftBody* body, short int collisionFilterGroup, short int collisionFilterMask ) {
	m_softBodies.push_back( body );

	// Set the soft body solver that will deal with this body
	// to be the world's solver
	body->setSoftBodySolver( m_softBodySolver );

	::btCollisionWorld::addCollisionObject( body, collisionFilterGroup, collisionFilterMask );
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::removeSoftBody( ::btSoftBody* body ) {
	m_softBodies.remove( body );
	BaseType::removeCollisionObject( body );
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::removeCollisionObject( ::btCollisionObject* collisionObject ) {
	if( ::btSoftBody* body = btSoftBody::upcast( collisionObject ) ) {
		removeSoftBody( body );
	} else {
		BaseType::removeCollisionObject( collisionObject );
	}
}

// ---------------------------------------------------

template <typename BaseType>
int btSoftBodyWorldMixin<BaseType>::getDrawFlags() const {
	return (m_drawFlags);
}

// ---------------------------------------------------

template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::setDrawFlags( int f ) {
	m_drawFlags = f;
}

// ---------------------------------------------------

template <typename BaseType>
::btSoftBodyWorldInfo& btSoftBodyWorldMixin<BaseType>::getWorldInfo() {
	return m_sbi;
}

// ---------------------------------------------------

template <typename BaseType>
const ::btSoftBodyWorldInfo& btSoftBodyWorldMixin<BaseType>::getWorldInfo() const {
	return m_sbi;
}

// ---------------------------------------------------

template <typename BaseType>
::btDynamicsWorldType btSoftBodyWorldMixin<BaseType>::getWorldType() const {
	return BT_SOFT_RIGID_DYNAMICS_WORLD;
}

// ---------------------------------------------------

template <typename BaseType>
::btAlignedObjectArray<::btSoftBody*>& btSoftBodyWorldMixin<BaseType>::getSoftBodyArray() {
	return m_softBodies;
}

// ---------------------------------------------------

template <typename BaseType>
const ::btAlignedObjectArray<::btSoftBody*>& btSoftBodyWorldMixin<BaseType>::getSoftBodyArray() const {
	return m_softBodies;
}

// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
//	MSVC static analysis doesn't correctly figure out that operator= is deleted for btSoftSingleRayCallback here and complains that it's been left empty.
#	pragma warning( push )
#	pragma warning( disable : 4822 )
#endif
template <typename BaseType>
void btSoftBodyWorldMixin<BaseType>::rayTest( const ::btVector3& rayFromWorld, const ::btVector3& rayToWorld, ::btCollisionWorld::RayResultCallback& resultCallback ) const {
	struct btSoftSingleRayCallback : public ::btBroadphaseRayCallback {
		using ThisType = ::btSoftBodyWorldMixin<BaseType>;

	// ---

		//!	Constructs this @ref btSoftSingleRayCallback instance.
		btSoftSingleRayCallback( const ::btVector3& rayFromWorld, const ::btVector3& rayToWorld, const ThisType* world, ::btCollisionWorld::RayResultCallback& resultCallback ) : m_rayFromWorld( rayFromWorld ),
																																												  m_rayToWorld( rayToWorld ),
																																												  m_world( world ),
																																												  m_resultCallback( resultCallback ) {
			m_rayFromTrans.setIdentity();
			m_rayFromTrans.setOrigin( m_rayFromWorld );
			m_rayToTrans.setIdentity();
			m_rayToTrans.setOrigin( m_rayToWorld );

			::btVector3	rayDir = (rayToWorld - rayFromWorld);

			rayDir.normalize();
			/// what about division by zero? --> just set rayDirection[i] to INF/1e30
			m_rayDirectionInverse[0] = rayDir[0] == btScalar( 0.0 ) ? btScalar( 1e30 ) : btScalar( 1.0 ) / rayDir[0];
			m_rayDirectionInverse[1] = rayDir[1] == btScalar( 0.0 ) ? btScalar( 1e30 ) : btScalar( 1.0 ) / rayDir[1];
			m_rayDirectionInverse[2] = rayDir[2] == btScalar( 0.0 ) ? btScalar( 1e30 ) : btScalar( 1.0 ) / rayDir[2];
			m_signs[0] = m_rayDirectionInverse[0] < 0.0;
			m_signs[1] = m_rayDirectionInverse[1] < 0.0;
			m_signs[2] = m_rayDirectionInverse[2] < 0.0;

			m_lambda_max = rayDir.dot( m_rayToWorld - m_rayFromWorld );
		}

	// ---------------------------------------------------

		virtual bool process( const ::btBroadphaseProxy* proxy ) override sealed {
			/// terminate further ray tests, once the closestHitFraction reached zero
			if( m_resultCallback.m_closestHitFraction == ::btScalar( 0.f ) )
				return false;

			::btCollisionObject*	collisionObject = static_cast<::btCollisionObject*>(proxy->m_clientObject);

			// only perform raycast if filterMask matches
			if( m_resultCallback.needsCollision( collisionObject->getBroadphaseHandle() ) ) {
				m_world->rayTestSingle( m_rayFromTrans, m_rayToTrans, collisionObject, collisionObject->getCollisionShape(), collisionObject->getWorldTransform(), m_resultCallback );
			}

			return true;
		}

	// ---------------------------------------------------

	private:
		btSoftSingleRayCallback&	operator=(const btSoftSingleRayCallback&) = delete;

	// ---------------------------------------------------

		::btVector3								m_rayFromWorld;
		::btVector3								m_rayToWorld;
		::btTransform							m_rayFromTrans;
		::btTransform							m_rayToTrans;
		::btVector3								m_hitNormal;

		const ThisType*							m_world;
		::btCollisionWorld::RayResultCallback&	m_resultCallback;
	};

// ---

	BT_PROFILE( "rayTest" );
	/// use the broadphase to accelerate the search for objects, based on their aabb
	/// and for each object with ray-aabb overlap, perform an exact ray test
	btSoftSingleRayCallback	rayCB( rayFromWorld, rayToWorld, this, resultCallback );

#ifndef USE_BRUTEFORCE_RAYBROADPHASE
	m_broadphasePairCache->rayTest( rayFromWorld, rayToWorld, rayCB );
#else
	for( int i = 0; i < this->getNumCollisionObjects(); i++ ) {
		rayCB.process( m_collisionObjects[i]->getBroadphaseHandle() );
	}
#endif //USE_BRUTEFORCE_RAYBROADPHASE
}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

// ---------------------------------------------------

template <typename BaseType>
void  btSoftBodyWorldMixin<BaseType>::rayTestSingle( const ::btTransform& rayFromTrans, const ::btTransform& rayToTrans, ::btCollisionObject* collisionObject, const ::btCollisionShape* collisionShape, const ::btTransform& colObjWorldTransform, ::btCollisionWorld::RayResultCallback& resultCallback ) {
	if( collisionShape->isSoftBody() ) {
		if( ::btSoftBody* softBody = btSoftBody::upcast( collisionObject ) ) {
			::btSoftBody::sRayCast	softResult;

			if( softBody->rayTest( rayFromTrans.getOrigin(), rayToTrans.getOrigin(), softResult ) ) {
				if( softResult.fraction <= resultCallback.m_closestHitFraction ) {
					::btCollisionWorld::LocalShapeInfo	shapeInfo;

					shapeInfo.m_shapePart		= 0;
					shapeInfo.m_triangleIndex	= softResult.index;
					// get the normal
					::btVector3 rayDir	= rayToTrans.getOrigin() - rayFromTrans.getOrigin();
					::btVector3 normal	= -rayDir;
					normal.normalize();

					if( softResult.feature == btSoftBody::eFeature::Face ) {
						normal = softBody->m_faces[softResult.index].m_normal;
						if( normal.dot( rayDir ) > 0 ) {
							// normal always point toward origin of the ray
							normal = -normal;
						}
					}

					::btCollisionWorld::LocalRayResult rayResult( collisionObject, &shapeInfo, normal, softResult.fraction );

					bool	normalInWorldSpace = true;
					resultCallback.addSingleResult( rayResult, normalInWorldSpace );
				}
			}
		}
	} else {
		::btCollisionWorld::rayTestSingle( rayFromTrans, rayToTrans, collisionObject, collisionShape, colObjWorldTransform, resultCallback );
	}
}

// ---------------------------------------------------

template <typename BaseType>
void  btSoftBodyWorldMixin<BaseType>::serialize( ::btSerializer* serializer ) {
	serializer->startSerialization();

	serializeDynamicsWorldInfo( serializer );

	serializeSoftBodies( serializer );

	serializeRigidBodies( serializer );

	serializeCollisionObjects( serializer );

	serializer->finishSerialization();
}