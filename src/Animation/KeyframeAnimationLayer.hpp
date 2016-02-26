/*==================================================================*\
  KeyframeAnimationLayer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationLayer.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		class	KeyframeAnimationResourceView;
	}
}

namespace Eldritch2 {
namespace Animation {

	class KeyframeAnimationLayer : public Animation::AnimationLayer {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------
	
	protected:
		//! Constructs this @ref KeyframeAnimationLayer instance.
		KeyframeAnimationLayer( Animation::KeyframeAnimationResourceView& asset );
		//! Constructs this @ref KeyframeAnimationLayer instance.
		KeyframeAnimationLayer( const KeyframeAnimationLayer& ) = default;
	
		~KeyframeAnimationLayer() = default;
	
	// ---------------------------------------------------
	
		void	EvaluateCacheForTime( Animation::KeyCache& cache, Animation::BoneIndex maximumBoneToConsider, ::Eldritch2::uint64 time ) override;
	
	// - DATA MEMBERS ------------------------------------
	
	public:
		static const char* const					scriptTypeName;
	
	private:
		Animation::KeyframeAnimationResourceView*	_asset;
		::Eldritch2::float32						_playbackRate;
	};

}	// namespace Animation
}	// namespace Eldritch2