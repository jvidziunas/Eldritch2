/*==================================================================*\
  LocalSpaceAnimationLayer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Animation {

	class LocalSpaceAnimator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		LocalSpaceAnimator( const ::Eldritch2::uint64 animationPeriodOffset, const ::Eldritch2::uint32 animationPeriod );

	// ---------------------------------------------------

		void	EvaluateForTime( const ::Eldritch2::uint16 maximumBoneToConsider, const ::Eldritch2::uint64 time );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::uint64	_animationPeriodOffset;
		::Eldritch2::uint32	_animationPeriod;
	};

// ---

	class LocalSpaceAdditiveKeyframeAnimationLayer : public Animation::Armature::AdditiveKeyframeAnimationLayer, protected Animation::LocalSpaceAnimator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this LocalSpaceAdditiveKeyframeAnimationLayer instance.
		LocalSpaceAdditiveKeyframeAnimationLayer( const ::Eldritch2::uint64 animationStartTime, const ::Eldritch2::uint32 animationLength, Animation::Armature::ReplacementKeyframeAnimationLayer& influencedLayer );
		// Constructs this LocalSpaceAdditiveKeyframeAnimationLayer instance.
		LocalSpaceAdditiveKeyframeAnimationLayer( const Animation::LocalSpaceAdditiveKeyframeAnimationLayer& ) = delete;

		// Destroys this LocalSpaceAdditiveKeyframeAnimationLayer instance.
		~LocalSpaceAdditiveKeyframeAnimationLayer() = default;

	// ---------------------------------------------------

		void	EvaluatePoseForTime( const ::Eldritch2::uint16 maximumBoneToConsider, const ::Eldritch2::uint64 time ) override sealed;
	};

// ---

	class LocalSpaceReplacementKeyframeAnimationLayer : public Animation::Armature::ReplacementKeyframeAnimationLayer, protected Animation::LocalSpaceAnimator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this LocalSpaceReplacementKeyframeAnimationLayer instance.
		LocalSpaceReplacementKeyframeAnimationLayer( const ::Eldritch2::uint64 animationStartTime, const ::Eldritch2::uint32 animationLength, Animation::Armature& influencedArmature );
		// Constructs this LocalSpaceReplacementKeyframeAnimationLayer instance.
		LocalSpaceReplacementKeyframeAnimationLayer( const Animation::LocalSpaceReplacementKeyframeAnimationLayer& ) = delete;

		// Destroys this LocalSpaceReplacementKeyframeAnimationLayer instance.
		~LocalSpaceReplacementKeyframeAnimationLayer() = default;

	// ---------------------------------------------------

		void	EvaluatePoseForTime( const ::Eldritch2::uint16 maximumBoneToConsider, const ::Eldritch2::uint64 time ) override sealed;
	};

}	// namespace Animation
}	// namespace Eldritch2