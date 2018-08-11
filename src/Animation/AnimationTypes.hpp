/*==================================================================*\
  AnimationTypes.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	using BoneIndex = int16;
	using KeyTime   = float16;

	// ---

	class KnotCache {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		KnotCache(const KnotCache&) = delete;
		//!	Constructs this @ref KnotCache instance.
		KnotCache();

		~KnotCache() = default;

		// ---------------------------------------------------

	public:
		template <uint32 knotCount>
		void PushKnots(uint32 index, const float32 (&startTimes)[knotCount], const float32 (&endTimes)[knotCount], const uint16 (&startValues)[knotCount], const uint16 (&endValues)[knotCount], const uint16 (&startTangents)[knotCount], const uint16 (&endTangents)[knotCount]);
		void PushKnots(uint32 index, uint32 channelCount, const float32* startTimes, const float32* endTimes, const uint16* startValues, const uint16* endValues, const uint16* startTangents, const uint16* endTangents);

		void PushKnot(uint32 index, float32 startTime, float32 endTime, uint16 startValue, uint16 endValue, uint16 startTangent, uint16 endTangent);

		// ---------------------------------------------------

	public:
		const uint32* GetTargetChannels() const;

		const float32* GetStartTimes() const;

		const float32* GetReciprocalDurations() const;

		const uint16* GetStartValues() const;

		const uint16* GetEndValues() const;

		const uint16* GetStartTangents() const;

		const uint16* GetEndTangents() const;

		uint32 GetSize() const;

		// ---------------------------------------------------

	public:
		uint32 AllocateKnots(uint32 knotCount, const uint32* channelIndices);
		template <uint32 knotCount>
		uint32 AllocateKnots(const uint32 (&channelIndices)[knotCount]);

		// ---------------------------------------------------

	private:
		void Resize(uint32 knots);

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<char> _memory;
		uint32          _capacity;
		uint32          _count;
		uint32*         _channelIndices;
		float32*        _startTimes;
		float32*        _reciprocalDurations;
		uint16*         _startValues;
		uint16*         _endValues;
		uint16*         _startTangents;
		uint16*         _endTangents;
	};

	// ---

	class ETPureAbstractHint Blend {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Blend instance.
		Blend(const Blend&) = default;
		//!	Constructs this @ref Blend instance.
		Blend() = default;

		virtual ~Blend() = default;

		// ---------------------------------------------------

	public:
		virtual void Evaluate(float32 values[], const float32 palette[], BoneIndex maximumBone) const abstract;
	};

	// ---

	class ETPureAbstractHint Clip : public Blend {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Clip instance.
		Clip(const Clip&) = default;
		//! Constructs this @ref Clip instance.
		Clip() = default;

		virtual ~Clip() = default;

		// ---------------------------------------------------

	public:
		void Evaluate(float32 values[], const float32 palette[], BoneIndex maximumBone) const override;

		// ---------------------------------------------------

	public:
		virtual void FetchKnots(KnotCache& knots, uint64 time, BoneIndex maximumBone) abstract;

		// ---------------------------------------------------

	public:
		void Attach(KnotCache& knots, uint32 knotCount);

		// - DATA MEMBERS ------------------------------------

	protected:
		uint32 _firstKnot;
	};

	// ---

	void EvaluatePose(Transformation poseToLocals[], Transformation poseToVelocity[], float32 scratch[], const Blend& blend, const KnotCache& knots, BoneIndex maximumBone, float32 time);
	void EvaluatePose(Transformation poseToLocals[], float32 scratch[], const Blend& blend, const KnotCache& knots, BoneIndex maximumBone, float32 time);

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AnimationTypes.inl>
//------------------------------------------------------------------//
