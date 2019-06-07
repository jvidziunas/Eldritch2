/*==================================================================*\
  AnimationTreeAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AssetViews/AnimationTreeAsset.hpp>
#include <Flatbuffers/FlatBufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/AnimationTree_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	
	// ---------------------------------------------------

	AnimationTreeAsset::AnimationTreeAsset(StringSpan path) ETNoexceptHint : Asset(path),
																			 _clips(ClipList::AllocatorType("Animation Tree Asset Clip Definitions Allocator")),
																			 _blends(BlendList::AllocatorType("Animation Tree Asset Blend Definitions Allocator")) {}

	// ---------------------------------------------------

	Result AnimationTreeAsset::BindResources(Log& log, const AssetBuilder& asset) {
		using namespace ::Eldritch2::Animation::FlatBuffers;
		using namespace ::flatbuffers;

		const auto ParseClips([](ClipList& outClips, const flatbuffers::Vector<Offset<ClipDescriptor>>* clips) -> Result {
			static ETConstexpr auto ParseClip([](const ClipDescriptor* /*clip*/) ETNoexceptHint -> Clip {
				return Clip{};
			});

			for (uoffset_t clip(0u); clip < clips->size(); ++clip) {
				outClips.EmplaceBack(ParseClip(clips->Get(clip)));
			}

			return Result::Success;
		});

		const auto ParseBlends([](BlendList& outBlends, const flatbuffers::Vector<Offset<BlendDescriptor>>* blends) -> Result {
			static ETConstexpr auto ParseBlend([](const BlendDescriptor* /*blend*/) ETNoexceptHint -> Blend {
				return Blend{};
			});

			for (uoffset_t blend(0u); blend < blends->size(); ++blend) {
				outBlends.EmplaceBack(ParseBlend(blends->Get(blend)));
			}

			return Result::Success;
		});

		// ---

		//	Ensure the data we're working with can plausibly represent an animation tree.
		const auto tree(GetVerifiedRoot<AnimationTreeDescriptor>(asset.bytes, AnimationTreeDescriptorIdentifier()));
		ET_ABORT_UNLESS(tree ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		ClipList clips(_clips.GetAllocator(), tree->Clips()->size());
		ET_ABORT_UNLESS(ParseClips(clips, tree->Clips()));

		BlendList blends(_blends.GetAllocator(), tree->Blends()->size());
		ET_ABORT_UNLESS(ParseBlends(blends, tree->Blends()));

		Swap(_clips, clips);
		Swap(_blends, blends);

		return Result::Success;
	}

	// ---------------------------------------------------

	void AnimationTreeAsset::FreeResources() ETNoexceptHint {
		_clips.Clear(ReleaseMemorySemantics());
		_blends.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan AnimationTreeAsset::GetExtension() ETNoexceptHint {
		using namespace ::Eldritch2::Animation::FlatBuffers;

		return StringSpan(AnimationTreeDescriptorExtension(), StringSpan::SizeType(StringLength(AnimationTreeDescriptorExtension())));
	}

}}} // namespace Eldritch2::Animation::AssetViews
