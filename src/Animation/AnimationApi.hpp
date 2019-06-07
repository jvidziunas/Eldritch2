/*==================================================================*\
  AnimationApi.hpp
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

	using SoaTransformation = float32[7];

	// ---

	enum class BoneIndex : uint16 {
		Root = 0x0000,
	};

	// ---

	enum class ClipTime : uint16 {
		Start = 0x0000,
		End   = 0xFFFF,
	};

	// ---

	enum class LoopMode : bool {
		Repeat,
		Hold,
	};

	// ---

	class ArmatureDefinition {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Joint {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Joint instance.
			Joint(const Joint&) ETNoexceptHint = default;
			//!	Constructs this @ref Joint instance.
			Joint() ETNoexceptHint = default;

			~Joint() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Transformation       bindPose;
			Animation::BoneIndex parent;
			struct {
				float32 min, max;
			} distanceLimits[3];
			struct {
				float32 min, max;
			} twistLimits[3];
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ArmatureDefinition(const ArmatureDefinition&) = delete;
		//!	Constructs this @ref ArmatureDefinition instance.
		ArmatureDefinition(ArmatureDefinition&&) = default;
		//!	Constructs this @ref ArmatureDefinition instance.
		ArmatureDefinition();

		~ArmatureDefinition() = default;

		// ---------------------------------------------------

	public:
		Span<const SoaTransformation*> GetBindPose() const ETNoexceptHint;

		Span<const Joint*> GetJoints() const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<float32> _bindPose;
		ArrayList<Joint>   _joints;
	};

	// ---

	class Armature {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Armature instance.
		ETConstexpr Armature(Transformation localToWorld) ETNoexceptHint;
		//! Disable copy construction.
		Armature(const Armature&) = delete;

		~Armature() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr Transformation ETSimdCall GetLocalToWorld() const ETNoexceptHint;

		ETConstexpr void ETSimdCall SetLocalToWorld(Transformation localToWorld) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy construction.
		Armature& operator=(const Armature&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Transformation _localToWorld;
	};

	// ---

	class ETPureAbstractHint Clip {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Clip instance.
		ETConstexpr Clip(MicrosecondTime startTime) ETNoexceptHint;
		//! Constructs this @ref Clip instance.
		ETConstexpr Clip(const Clip&) ETNoexceptHint = default;

		virtual ~Clip() = default;

		// ---------------------------------------------------

	public:
		virtual void Evaluate(MicrosecondTime worldTime, BoneIndex maximumBone, SoaTransformation pose[]) ETNoexceptHint abstract;

		// ---------------------------------------------------

	public:
		ETConstexpr MicrosecondTime GetStartTime() const ETNoexceptHint;

		ETConstexpr void SetStartTime(MicrosecondTime worldTime) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		MicrosecondTime _startTime;

		// ---------------------------------------------------

		friend void Swap(Clip&, Clip&) = delete;
	};

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AnimationApi.inl>
//------------------------------------------------------------------//
