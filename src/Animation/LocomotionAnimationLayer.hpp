/*==================================================================*\
  LocomotionAnimationLayer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Armature.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Animation {

	namespace Scripting = ::Eldritch2::Scripting;
	namespace Animation = ::Eldritch2::Animation;

// ---------------------------------------------------

	class BipedalLocomotionAnimationLayer : public Animation::Armature::ReplacementKeyframeAnimationLayer {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct FootstepState {
			::Eldritch2::uint64		endTime;
			::Eldritch2::Float4		centerOfMassPosition;
			::Eldritch2::float16	centerOfMassVelocity[3];
		};

	// ---

		enum ActiveFoot : bool {
			LEFT,
			RIGHT
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this BipedalLocomotionAnimationLayer instance.
		BipedalLocomotionAnimationLayer( Animation::Armature& armature );
		// Constructs this BipedalLocomotionAnimationLayer instance.
		BipedalLocomotionAnimationLayer( const Animation::BipedalLocomotionAnimationLayer& ) = delete;

		// Destroys this BipedalLocomotionAnimationLayer instance.
		~BipedalLocomotionAnimationLayer() = default;

	// ---------------------------------------------------

		::Eldritch2::Float4	EvaluateRootDisplacementDeltaForTime( const ::Eldritch2::uint64 lastEvalutedTime, const ::Eldritch2::uint64 currentTime ) override;

		void				EvaluatePoseForTime( const ::Eldritch2::uint16 maximumBoneToConsider, const ::Eldritch2::uint64 time ) override sealed;

	// - DATA MEMBERS ------------------------------------

	private:
		FootstepState	_footStates[2];
	};

}	// namespace Animation
}	// namespace Eldritch2