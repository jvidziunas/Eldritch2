/*==================================================================*\
  KeyframeClip.cpp
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
#include <Animation/KeyframeClip.hpp>
//------------------------------------------------------------------//
#include <Animation/AnimateSkeleton_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	using namespace ::Eldritch2::Animation::AssetViews;

	// ---------------------------------------------------

	void KeyframeClip::Evaluate(MicrosecondTime worldTime, BoneIndex maximumBone, SoaTransformation pose[]) ETNoexceptHint {
		static ETConstexpr uint32 TracksPerBone(ETCountOf(pose[0]));

		const ClipTime now(AsLocalTime(worldTime));
		UpdateCache(now);

		ispc::BulkHermite(uint32(maximumBone) * TracksPerBone,
						  /*time =*/uint16(now),
						  reinterpret_cast<float32*>(pose),
						  /*channels =*/nullptr,
						  /*times =*/nullptr,
						  /*durations =*/nullptr,
						  /*startValues =*/nullptr,
						  /*endValues =*/nullptr,
						  /*startTangents =*/nullptr,
						  /*endTangents =*/nullptr);
	}

	// ---------------------------------------------------

	void KeyframeClip::UpdateCache(ClipTime time) ETNoexceptHint {
		if (_validPeriod.Covers(time)) {
			return; // Cache is still valid, don't need to update.
		}

		const auto translations(_asset->GetTranslations());
		const auto nextTranslation(UpperBound(translations.Begin(), translations.End(), time, [](ClipTime time, const KeyframeClipAsset::TranslationKey& key) ETNoexceptHint {
			return time < key.time;
		}));

		const auto orientations(_asset->GetOrientations());
		const auto nextOrientation(UpperBound(orientations.Begin(), orientations.End(), time, [](ClipTime time, const KeyframeClipAsset::OrientationKey& key) ETNoexceptHint {
			return time < key.time;
		}));

		_validPeriod = Interval{ time, time }; // FIXME: This is correct, but *extremely* conservative.
	}

}} // namespace Eldritch2::Animation
