/*==================================================================*\
  Physics.AnimationClip.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/Physics.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <PxArticulationLink.h>
#	include <PxRigidBody.h>
#	include <PxPhysics.h>
#	include <PxScene.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

using namespace ::Eldritch2::Physics::PhysX::AssetViews;
using namespace ::Eldritch2::Animation;
using namespace ::physx;
using namespace ::ispc;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace {

	static ETInlineHint ETPureFunctionHint Transformation ETSimdCall Integrate( Transformation baseTransform ) {
		return baseTransform;
	}

}	// anonymous namespace

	Physics::AnimationClip::AnimationClip( const PxArticulation& articulation ) : _articulation( &articulation ) {}

// ---------------------------------------------------

	void Physics::AnimationClip::PrefetchTransforms( uint64 /*timeBegin*/, uint64 /*timeEnd*/ ) {
	//	Physics-derived animation does not reference transform tracks, so this is a no-op.
	}

// ---------------------------------------------------

	void Physics::AnimationClip::EvaluateWorldPose( Transformation localToWorld, GpuTransformWithVelocity transforms[] ) const {
		PxArticulationLink** const links( static_cast<PxArticulationLink**>(_alloca( _articulation->getNbLinks() * sizeof(PxArticulationLink*) )) );

		_articulation->getLinks( links, _articulation->getNbLinks() );

		for (BoneIndex target( 0u ), end( _articulation->getNbLinks() ); target < end; ++target) {
			const auto	parentToLocal( AsTransformation( links[target]->getInboundJoint()->getParentPose() ) );
		}
	}

// ---------------------------------------------------

	void Physics::AnimationClip::EvaluateWorldPose( Transformation localToWorld, GpuTransform transforms[] ) const {
		const auto	links( static_cast<PxArticulationLink**>( _alloca( _articulation->getNbLinks() * sizeof(PxArticulationLink*) ) ) );

		_articulation->getLinks( links, _articulation->getNbLinks() );

		for (BoneIndex target( 0u ), end( _articulation->getNbLinks() ); target < end; ++target) {
			const auto	parentToLocal( AsTransformation( links[target]->getInboundJoint()->getParentPose() ) );
		}
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2