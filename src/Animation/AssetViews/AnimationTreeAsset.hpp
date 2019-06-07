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
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	class AnimationTreeAsset : public Core::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct Blend {};
		struct Clip {};

		// ---

	public:
		using ClipList  = ArrayList<Clip>;
		using BlendList = ArrayList<Blend>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		AnimationTreeAsset(const AnimationTreeAsset&) = delete;
		//!	Constructs this @ref AnimationTreeAsset instance.
		AnimationTreeAsset(StringSpan path) ETNoexceptHint;

		~AnimationTreeAsset() override = default;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		ClipList  _clips;
		BlendList _blends;
	};

}}} // namespace Eldritch2::Animation::AssetViews
