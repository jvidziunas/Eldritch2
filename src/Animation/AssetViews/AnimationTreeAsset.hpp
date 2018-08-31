/*==================================================================*\
  AnimationTreeAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	class AnimationTreeAsset : public Assets::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct Blend {};
		struct Clip {};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		AnimationTreeAsset(const AnimationTreeAsset&) = delete;
		//!	Constructs this @ref AnimationTreeAsset instance.
		AnimationTreeAsset(StringView path);

		~AnimationTreeAsset() override = default;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		ArrayList<Clip>  _clips;
		ArrayList<Blend> _blends;
	};

}}} // namespace Eldritch2::Animation::AssetViews
