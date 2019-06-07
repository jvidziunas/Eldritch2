/*==================================================================*\
  KeyframeClipAsset.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	ETInlineHint ETForceInlineHint Span<const KeyframeClipAsset::TranslationKey*> KeyframeClipAsset::GetTranslations() const ETNoexceptHint {
		return _translations;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Span<const KeyframeClipAsset::OrientationKey*> KeyframeClipAsset::GetOrientations() const ETNoexceptHint {
		return _orientations;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Span<const KeyframeClipAsset::EventKey*> KeyframeClipAsset::GetEvents() const ETNoexceptHint {
		return _events;
	}

}}}	// namespace Eldritch2::Animation::AssetViews
