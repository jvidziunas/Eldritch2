/*==================================================================*\
  KeyframeClipAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AssetViews/KeyframeClipAsset.hpp>
#include <Flatbuffers/FlatbufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/Animation_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	KeyframeClipAsset::KeyframeClipAsset(StringSpan path) ETNoexceptHint : Asset(path),
																		   _allocator("Keyframe Clip Root Allocator"),
																		   _translations(TranslationKeyList::AllocatorType(_allocator, "Keyframe Clip Translation Key List Allocator")),
																		   _orientations(OrientationKeyList::AllocatorType(_allocator, "Keyframe Clip Orientation Key List Allocator")),
																		   _events(EventKeyList::AllocatorType(_allocator, "Keyframe Clip Event Key List Allocator")) {}

	// ---------------------------------------------------

	Result KeyframeClipAsset::BindResources(Log& log, const AssetBuilder& asset) {
		using namespace ::Eldritch2::Animation::FlatBuffers;
		using namespace ::flatbuffers;

		static MallocAllocator KeyframeClipRootAllocator("Keyframe Clip Root Allocator");

		static ETConstexpr auto ParseTranslationKeys([](TranslationKeyList& translations, const flatbuffers::Vector<const FlatBuffers::TranslationKey*>* asset) ETNoexceptHint -> Result {
			translations.SetCapacity(asset->size());
			return Result::Success;
		});

		static ETConstexpr auto ParseOrientationKeys([](OrientationKeyList& orientations, const flatbuffers::Vector<const FlatBuffers::OrientationKey*>* asset) ETNoexceptHint -> Result {
			orientations.SetCapacity(asset->size());
			return Result::Success;
		});

		static ETConstexpr auto ParseEventKeys([](EventKeyList& events, const flatbuffers::Vector<Offset<FlatBuffers::EventKey>>* asset) ETNoexceptHint -> Result {
			events.SetCapacity(asset->size());
			return Result::Success;
		});

		// ---

		//	Ensure we're working with data that can plausibly represent an animation clip.
		const auto animation(GetVerifiedRoot<AnimationDescriptor>(asset.bytes, AnimationDescriptorIdentifier()));
		ET_ABORT_UNLESS(animation ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		// Backing memory for all keyframes is allocated in a single top-level chunk, then split by type
		ArenaChildAllocator allocator(_allocator.GetName());
		ET_ABORT_UNLESS(allocator.BindResources(KeyframeClipRootAllocator, /*byteSize =*/16u));

		TranslationKeyList translations(TranslationKeyList::AllocatorType(allocator, _translations.GetAllocator().GetName()));
		ET_ABORT_UNLESS(ParseTranslationKeys(translations, animation->TranslationKeys()));

		OrientationKeyList orientations(OrientationKeyList::AllocatorType(allocator, _orientations.GetAllocator().GetName()));
		ET_ABORT_UNLESS(ParseOrientationKeys(orientations, animation->OrientationKeys()));

		EventKeyList events(EventKeyList::AllocatorType(allocator, _events.GetAllocator().GetName()));
		ET_ABORT_UNLESS(ParseEventKeys(events, animation->EventKeys()));

		Swap(_allocator, allocator);
		Swap(_translations, translations);
		Swap(_orientations, orientations);
		Swap(_events, events);

		return Result::Success;
	}

	// ---------------------------------------------------

	void KeyframeClipAsset::FreeResources() ETNoexceptHint {
		_events.Clear(ReleaseMemorySemantics());
		_orientations.Clear(ReleaseMemorySemantics());
		_translations.Clear(ReleaseMemorySemantics());
		_allocator.FreeResources();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan KeyframeClipAsset::GetExtension() ETNoexceptHint {
		using namespace ::Eldritch2::Animation::FlatBuffers;

		return StringSpan(AnimationDescriptorExtension(), StringSpan::SizeType(StringLength(AnimationDescriptorExtension())));
	}

}}} // namespace Eldritch2::Animation::AssetViews
