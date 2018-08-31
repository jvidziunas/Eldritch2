/*==================================================================*\
  KeyframeClipAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/KeyframeClip.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	class KeyframeClipAsset : public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		KeyframeClipAsset(const KeyframeClipAsset&) = delete;
		//!	Constructs this @ref KeyframeClipAsset instance.
		KeyframeClipAsset(StringView path);

		~KeyframeClipAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		KeyframeClipAsset& operator=(const KeyframeClipAsset&) = delete;
	};

}}} // namespace Eldritch2::Animation::AssetViews
