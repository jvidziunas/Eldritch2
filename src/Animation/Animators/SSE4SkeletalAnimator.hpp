/*==================================================================*\
  SSE4SkeletalAnimator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Animation {

	class SSE4SkeletalAnimator {
		void	AnimateKeys( const ::Eldritch2::uint32 localAnimationTime, const ::Eldritch2::Range<const Animation::CachedQuaternionAnimationKey*> keys );
	};

}	// namespace Animation
}	// namespace Eldritch2