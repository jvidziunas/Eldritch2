/*==================================================================*\
  AnimationApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//
#include <AnimateSkeleton_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	ArmatureDefinition::ArmatureDefinition() :
		joints(MallocAllocator("Armature Definition Joints List Allocator")),
		socketByName(MallocAllocator("Armature Definition Socket/Name Map Allocator")) {}

	// ---------------------------------------------------

	KnotCache::KnotCache(SizeType capacityHint) :
		_knots(MallocAllocator("Knot Cache Backing Memory Allocator"), capacityHint) {
	}

	// ---------------------------------------------------

	void Clip::Evaluate(BoneIndex maximumBone, float32 values[], const float32 palette[]) const {
		ispc::CopyPalette(maximumBone, values, palette + _firstKnot);
	}

	// ---------------------------------------------------

	void Swap(ArmatureDefinition& lhs, ArmatureDefinition& rhs) {
		Swap(lhs.joints, rhs.joints);
		Swap(lhs.socketByName, rhs.socketByName);
	}

}} // namespace Eldritch2::Animation
