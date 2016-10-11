/*==================================================================*\
  SkeletalArmature.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/SkeletalArmature.hpp>
#include <Physics/PhysX/AssetViews/ArmatureView.hpp>
#include <Physics/PhysX/TypeMarshalers.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Animation/KeyCache.hpp>
//------------------------------------------------------------------//
#include <PxRigidBody.h>
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Physics::PhysX::AssetViews;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {
namespace {

	static ETInlineHint ETPureFunctionHint RigidTransform ETSimdCall IntegrateTransform( RigidTransform baseTransform ) {
		return baseTransform;
	}

}	// anonymous namespace

	SkeletalArmature::PhysicsAnimationLayer::PhysicsAnimationLayer( const ArmatureView& assetView, UniquePointer<PxArticulation> articulation ) : _assetView( assetView ), _articulation( eastl::move( articulation ) ) {}

// ---------------------------------------------------

	void SkeletalArmature::PhysicsAnimationLayer::EvaluateCacheForAlphaFactor( KeyCache& keyCache, BoneIndex maximumBoneToConsider, uint64 /*time*/ ) {
		const auto	links( static_cast<PxArticulationLink**>(_alloca( _articulation->getNbLinks() * sizeof(PxArticulationLink*) )) );

		_articulation->getLinks( links, _articulation->getNbLinks() );

	//	Copy the local-space bone transforms into the key cache for evaluation at arbitrary time.
		const RigidTransform	rootPose( RigidTransformFromPxTransform( links[0]->getGlobalPose() ) );

		keyCache.AddKey( { 0u, rootPose, IntegrateTransform( rootPose ), float16(0.0f), float16(1.0f) } );
	
		for( Animation::BoneIndex targetBone( 1u ), end( _articulation->getNbLinks() ); targetBone < end; ++targetBone ) {
			const RigidTransform	localPose( RigidTransformFromPxTransform( links[targetBone]->getInboundJoint()->getChildPose() ) );

			keyCache.AddKey( { targetBone, localPose, IntegrateTransform( localPose ), float16(0.0f), float16(1.0f) } );
		}
	}

// ---------------------------------------------------

	SkeletalArmature::SkeletalArmature( RigidTransform rootTransform, const ArmatureView& armatureView, UniquePointer<PxArticulation> articulation, Allocator& allocator ) : Armature( rootTransform, allocator ), _physicsAnimationLayer( armatureView, eastl::move( articulation ) ) {
		AttachLayer( _physicsAnimationLayer );
	}

// ---------------------------------------------------

	SkeletalArmature::~SkeletalArmature() {
		DetachLayer( _physicsAnimationLayer );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2