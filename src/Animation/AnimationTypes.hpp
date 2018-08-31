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

	using BoneIndex = uint16;
	using KeyTime   = float16;

	// ---

	class KnotCache {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ChannelIndex   = BoneIndex;
		using Time           = float32;
		using Key            = uint16;
		using Container      = typename SoArrayList<Time /*start*/, Time /*reciprocalDuration*/, Key /*startValue*/, Key /*endValue*/, Key /*startTangent*/, Key /*endTangent*/, ChannelIndex /*channel*/>;
		using ConstPointer   = typename Container::ConstPointer;
		using Pointer        = typename Container::Pointer;
		using ConstReference = typename Container::ConstReference;
		using Reference      = typename Container::Reference;
		using SizeType       = typename Container::SizeType;

		enum : size_t {
			StartTimes = 0,
			ReciprocalDurations,
			StartValues,
			EndValues,
			StartTangents,
			EndTangents,
			TargetChannels
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref KnotCache instance.
		KnotCache(SizeType capacityHint = 0u);
		//!	Constructs this @ref KnotCache instance.
		KnotCache(const KnotCache&) = default;
		//!	Constructs this @ref KnotCache instance.
		KnotCache(KnotCache&&) = default;

		~KnotCache() = default;

		// ---------------------------------------------------

	public:
		ConstReference operator[](SizeType offset) const ETNoexceptHint;
		Reference      operator[](SizeType offset) ETNoexceptHint;

		ConstPointer Get(SizeType offset) const ETNoexceptHint;
		Pointer      Get(SizeType offset) ETNoexceptHint;

		SizeType GetSize() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		SizeType Append(SizeType count, const ChannelIndex* channels);

		void Reserve(SizeType capacityHint);

		void Clear() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		Container _knots;

		// ---------------------------------------------------

		friend void Swap(KnotCache&, KnotCache&);
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
		virtual void Evaluate(BoneIndex maximumBone, float32 values[], const float32 palette[]) const abstract;

		// ---------------------------------------------------

	public:
		Blend& operator=(const Blend&) = default;
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
		void Evaluate(BoneIndex maximumBone, float32 values[], const float32 palette[]) const override;

		// ---------------------------------------------------

	public:
		virtual void FetchKnots(KnotCache& knots, BoneIndex maximumBone, uint64 time) abstract;

		virtual void Attach(KnotCache& knots) abstract;

		// - DATA MEMBERS ------------------------------------

	protected:
		uint32 _firstKnot;
	};

	// ---

	void EvaluatePose(BoneIndex maximumBone, float32 time, Transformation poseToLocals[], Transformation poseToVelocity[], float32 scratch[], const KnotCache& knots, const Blend& blend);
	void EvaluatePose(BoneIndex maximumBone, float32 time, Transformation poseToLocals[], float32 scratch[], const KnotCache& knots, const Blend& blend);

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AnimationTypes.inl>
//------------------------------------------------------------------//
