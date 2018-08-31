/*==================================================================*\
  AnimationTypes.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	KnotCache::KnotCache(SizeType capacityHint) :
		_knots(MallocAllocator("Knot Cache Backing Memory Allocator"), capacityHint) {
	}

	// ---------------------------------------------------

	void Clip::Evaluate(BoneIndex maximumBone, float32 values[], const float32 palette[]) const {
		ispc::CopyPalette(maximumBone, values, palette + _firstKnot);
	}

}} // namespace Eldritch2::Animation
