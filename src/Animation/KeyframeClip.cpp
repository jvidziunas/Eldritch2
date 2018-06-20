/*==================================================================*\
  KeyframeAnimationNode.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/KeyframeClip.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		namespace Detail {

			using namespace ::Eldritch2::Animation::AssetViews;

			AbstractKeyframeClip::AbstractKeyframeClip(const KeyframeClipAsset& asset) : _asset(&asset) {}

		}	// namespace Detail
	}	// namespace Animation
}	// namespace Eldritch2