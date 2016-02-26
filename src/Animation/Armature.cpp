/*==================================================================*\
  Armature.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Animation {

	const char* const Armature::scriptTypeName	= "Armature";

// ---------------------------------------------------

	void Armature::UpdateCachedKeys( BoneIndex maximumBoneToConsider, uint64 time ) {
		for( auto& layer : _layers ) {
			layer.EvaluateCacheForTime( _keyCache, maximumBoneToConsider, time );
		}
	}

}	// namespace Animation
}	// namespace Eldritch2