/*==================================================================*\
  AnimationTreeAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AssetViews/AnimationTreeAsset.hpp>
#include <Flatbuffers/FlatBufferTools.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/AnimationTree_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation { namespace AssetViews {

	using namespace ::Eldritch2::Animation::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;

	namespace {

		ETInlineHint ETForceInlineHint AnimationTreeAsset::Clip ParseClip(const ClipDescriptor* /*clip*/) ETNoexceptHint {
			return AnimationTreeAsset::Clip {};
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint AnimationTreeAsset::Blend ParseBlend(const BlendDescriptor* /*blend*/) ETNoexceptHint {
			return AnimationTreeAsset::Blend {};
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseClips(ArrayList<AnimationTreeAsset::Clip>& outClips, const FlatbufferVector<Offset<ClipDescriptor>>* clips) {
			for (uoffset_t clip(0u); clip < clips->size(); ++clip) {
				outClips.Append(ParseClip(clips->Get(clip)));
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseBlends(ArrayList<AnimationTreeAsset::Blend>& outBlends, const FlatbufferVector<Offset<BlendDescriptor>>* blends) {
			for (uoffset_t blend(0u); blend < blends->size(); ++blend) {
				outBlends.Append(ParseBlend(blends->Get(blend)));
			}

			return Error::None;
		}

	} // anonymous namespace

	AnimationTreeAsset::AnimationTreeAsset(StringView path) :
		Asset(path),
		_clips(MallocAllocator("Animation Tree Asset Clip Definitions Allocator")),
		_blends(MallocAllocator("Animation Tree Asset Blend Definitions Allocator")) {
	}

	// ---------------------------------------------------

	ErrorCode AnimationTreeAsset::BindResources(const Builder& asset) {
		//	Ensure the data we're working with can plausibly represent an animation tree.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyAnimationTreeBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const AnimationTree* const tree(GetAnimationTree(asset.Begin()));

		ArrayList<Clip> clips(_clips.GetAllocator());
		ET_ABORT_UNLESS(ParseClips(clips, tree->Clips()));

		ArrayList<Blend> blends(_blends.GetAllocator());
		ET_ABORT_UNLESS(ParseBlends(blends, tree->Blends()));

		Swap(_clips, clips);
		Swap(_blends, blends);

		return Error::None;
	}

	// ---------------------------------------------------

	void AnimationTreeAsset::FreeResources() {
		_clips.Clear(ReleaseMemorySemantics());
		_blends.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView AnimationTreeAsset::GetExtension() ETNoexceptHint {
		return { AnimationTreeExtension(), StringLength(AnimationTreeExtension()) };
	}

}}} // namespace Eldritch2::Animation::AssetViews
