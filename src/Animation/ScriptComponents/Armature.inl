/*==================================================================*\
  Armature.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
namespace ScriptComponents {

	ETInlineHint Eldritch2::RigidTransform ETSimdCall Armature::GetRootTransform() const {
		return _rootTransform;
	}

// ---------------------------------------------------

	ETInlineHint const Animation::KeyCache& Armature::GetKeyCache() const {
		return _keyCache;
	}

// ---------------------------------------------------

	ETInlineHint Animation::KeyCache& Armature::GetKeyCache() {
		return _keyCache;
	}

// ---------------------------------------------------

	ETInlineHint void Armature::AttachLayer( AnimationLayer& layer ) {
		_layers.PushBack( layer );
		_keyCache.Invalidate();
	}

// ---------------------------------------------------

	ETInlineHint void Armature::AttachChildLayer( AnimationLayer& layer, AnimationLayer& parent ) {
		_layers.Insert( _layers.IteratorTo( parent ), layer );
		_keyCache.Invalidate();
	}

// ---------------------------------------------------

	ETInlineHint void Armature::DetachLayer( AnimationLayer& layer ) {
		_layers.Erase( _layers.IteratorTo( layer ) );
		_keyCache.Invalidate();
	}

}	// namespace ScriptComponents
}	// namespace Animation
}	// namespace Eldritch2