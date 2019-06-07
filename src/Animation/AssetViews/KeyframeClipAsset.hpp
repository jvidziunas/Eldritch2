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
#include <Animation/AnimationApi.hpp>
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	class KeyframeClipAsset : public Core::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct TranslationKey {
			ClipTime time;
			uint16   track : 14;
			uint16   padding : 2;
			uint16   x, y, z;
		};

		// ---

		struct OrientationKey {
			ClipTime time;
			uint16   track : 14;
			uint16   largestIndex : 2;
			uint16   component0, component1, component2;
		};

		// ---

		struct EventKey {
			ClipTime time;
		};

		// ---

		using TranslationKeyList = ArrayList<TranslationKey, ChildAllocator>;
		using OrientationKeyList = ArrayList<OrientationKey, ChildAllocator>;
		using EventKeyList       = ArrayList<EventKey, ChildAllocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		KeyframeClipAsset(const KeyframeClipAsset&) = delete;
		//!	Constructs this @ref KeyframeClipAsset instance.
		KeyframeClipAsset(StringSpan path) ETNoexceptHint;

		~KeyframeClipAsset() override = default;

		// ---------------------------------------------------

	public:
		Span<const TranslationKey*> GetTranslations() const ETNoexceptHint;

		Span<const OrientationKey*> GetOrientations() const ETNoexceptHint;

		Span<const EventKey*> GetEvents() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		KeyframeClipAsset& operator=(const KeyframeClipAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArenaChildAllocator _allocator;
		TranslationKeyList  _translations;
		OrientationKeyList  _orientations;
		EventKeyList        _events;
	};

}}} // namespace Eldritch2::Animation::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AssetViews/KeyframeClipAsset.inl>
//------------------------------------------------------------------//
