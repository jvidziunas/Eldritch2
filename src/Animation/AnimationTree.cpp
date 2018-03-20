/*==================================================================*\
  AnimationTree.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTree.hpp>
#include <Scheduling/JobExecutor.hpp>
//------------------------------------------------------------------//
#include <Animation/AnimateSkeletons_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::ispc;

	AnimationTree::AnimationTree(
	) : _allocator( "Animation Graph Node Allocator" ),
		_clips( MallocAllocator( "Animation Graph Clip Collection Allocator" ) ),
		_blends( MallocAllocator( "Animation Graph Blend Collection Allocator" ) ) {}

// ---------------------------------------------------

	void AnimationTree::PrefetchClipTransforms( JobExecutor& /*executor*/, uint64 timeBegin, uint64 timeEnd ) {
		for (const UniquePointer<Clip>& clip : _clips) {
			clip->PrefetchTransforms( timeBegin, timeEnd );
		}
	}

// ---------------------------------------------------

	void AnimationTree::EvaluatePose( Transformation localToWorld, BoneIndex maximumBoneToAnimate, GpuTransformWithVelocity transforms[] ) const {
		_root->EvaluateGlobalPose( localToWorld, maximumBoneToAnimate, transforms );
	}

// ---------------------------------------------------

	void AnimationTree::EvaluatePose( Transformation localToWorld, BoneIndex maximumBoneToAnimate, GpuTransform transforms[] ) const {
		_root->EvaluateGlobalPose( localToWorld, maximumBoneToAnimate, transforms );
	}

// ---------------------------------------------------

	void AnimationTree::Reset() {
		_blends.Clear( ReleaseMemorySemantics() );
		_clips.Clear( ReleaseMemorySemantics() );
		_allocator.FreeResources();
	}

}	// namespace Animation
}	// namespace Eldritch2