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

	ETInlineHint void Armature::ReplacementKeyframeAnimationLayer::AttachAnimationLayer( Armature::AdditiveKeyframeAnimationLayer& layer ) {
		_attachedAdditiveAnimationLayers.PushFront( layer );
	}

// ---------------------------------------------------

	ETInlineHint void Armature::AttachAnimationLayer( Armature::ReplacementKeyframeAnimationLayer& layer ) {
		_topLevelAnimations.PushFront( layer );
	}

// ---------------------------------------------------

	ETInlineHint void Armature::AttachAnimationLayer( Armature::PostprocessAnimationLayer& layer ) {
		_postprocessAnimations.PushFront( layer );
	}

}	// namespace Animation
}	// namespace Eldritch2