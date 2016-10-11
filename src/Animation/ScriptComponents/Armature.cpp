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
#include <Animation/ScriptComponents/Armature.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Animation {
namespace ScriptComponents {

	Armature::Armature( RigidTransform rootTransform, Allocator& allocator ) : _rootTransform( rootTransform ), _keyCache( allocator ) {}

// ---------------------------------------------------

	void Armature::UpdateCachedKeys( BoneIndex maximumBoneToConsider, float16 alpha ) {
		if( _keyCache.IsValidForAlphaFactor( alpha ) ) {
			return;
		}

		_keyCache.Invalidate();

		for( AnimationLayer& layer : _layers ) {
			layer.EvaluateCacheForAlphaFactor( _keyCache, maximumBoneToConsider, alpha );
		}
	}

}	// namespace ScriptComponents
}	// namespace Animation
}	// namespace Eldritch2