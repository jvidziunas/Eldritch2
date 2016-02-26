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

	namespace Animation	= ::Eldritch2::Animation;

// ---------------------------------------------------

	ETInlineHint const Animation::KeyCache& Armature::GetKeyCache() const {
		return _keyCache;
	}

// ---------------------------------------------------

	ETInlineHint void Armature::AttachLayer( AnimationLayer& layer ) {
		layer.AddReference();

		_layers.PushFront( layer );
	}

// ---------------------------------------------------

	ETInlineHint void Armature::AttachChildLayer( AnimationLayer& layer, AnimationLayer& parent ) {
		layer.AddReference();

		_layers.InsertAfter( _layers.IteratorTo( parent ), layer );
	}

// ---------------------------------------------------

	ETInlineHint void Armature::DetachLayer( AnimationLayer& layer ) {
		_layers.EraseAndDispose( _layers.IteratorTo( layer ), [] ( AnimationLayer& disposingLayer ) {
			disposingLayer.ReleaseReference();
		} );
	}

}	// namespace Animation
}	// namespace Eldritch2