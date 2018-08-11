/*==================================================================*\
  AnimationTypes.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	KnotCache::KnotCache() :
		_memory(MallocAllocator("Knot Cache Backing Memory Allocator")),
		_capacity(0u),
		_count(0u),
		_channelIndices(nullptr),
		_startTimes(nullptr),
		_reciprocalDurations(nullptr),
		_startValues(nullptr),
		_endValues(nullptr),
		_startTangents(nullptr),
		_endTangents(nullptr) {
	}

	// ---------------------------------------------------

	void KnotCache::Resize(uint32 knots) {
		enum : size_t { ChannelSize = sizeof(uint32) + 2u * sizeof(float32) + 4u * sizeof(uint16) };

		ArrayList<char> memory(_memory.GetAllocator());

		memory.Resize(knots * ChannelSize);

		const size_t copyCount(Min(knots, _count));

		uint32*  targetChannels(reinterpret_cast<uint32*>(memory.GetData()));
		float32* startTimes(reinterpret_cast<float32*>(targetChannels + knots));
		float32* reciprocalDurations(startTimes + knots);
		uint16*  startValues(reinterpret_cast<uint16*>(reciprocalDurations + knots));
		uint16*  endValues(startValues + knots);
		uint16*  startTangents(endValues + knots);
		uint16*  endTangents(startTangents + knots);

		CopyArrayNonTemporal(targetChannels, GetTargetChannels(), copyCount);
		CopyArrayNonTemporal(startTimes, GetStartTimes(), copyCount);
		CopyArrayNonTemporal(reciprocalDurations, GetReciprocalDurations(), copyCount);
		CopyArrayNonTemporal(startValues, GetStartValues(), copyCount);
		CopyArrayNonTemporal(endValues, GetEndValues(), copyCount);
		CopyArrayNonTemporal(startTangents, GetStartTangents(), copyCount);
		CopyArrayNonTemporal(endTangents, GetEndTangents(), copyCount);

		Swap(_memory, memory);
		Swap(_channelIndices, targetChannels);
		Swap(_startTimes, startTimes);
		Swap(_reciprocalDurations, reciprocalDurations);
		Swap(_startValues, startValues);
		Swap(_endValues, endValues);
		Swap(_startTangents, startTangents);
		Swap(_endTangents, endTangents);
	}

	// ---------------------------------------------------

	void Clip::Evaluate(float32 values[], const float32 palette[], BoneIndex maximumBone) const {
		ispc::CopyPalette(values, palette + _firstKnot, maximumBone);
	}

}} // namespace Eldritch2::Animation
