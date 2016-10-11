/*==================================================================*\
  AnimationLayer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveListHook.hpp>
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		class	KeyCache;
	}
}

namespace Eldritch2 {
namespace Animation {

	class ETPureAbstractHint AnimationLayer : public Eldritch2::IntrusiveListBaseHook {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//! Constructs this @ref AnimationLayer instance.
		AnimationLayer( const AnimationLayer& ) = delete;
	//! Constructs this @ref AnimationLayer instance.
		AnimationLayer() = default;

		~AnimationLayer() = default;

	// ---------------------------------------------------

	public:
		virtual void	EvaluateCacheForAlphaFactor( Animation::KeyCache& keyCache, Animation::BoneIndex maximumBoneToConsider, Eldritch2::uint64 time ) abstract;
	};

}	// namespace Animation
}	// namespace Eldritch2
