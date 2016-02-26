/*==================================================================*\
  BipedalLocomotionAnimationLayer.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationLayer.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	class BipedalLocomotionAnimationLayer : public Animation::AnimationLayer {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct FootstepState {
			::Eldritch2::uint64		endTime;
			::Eldritch2::Float4		centerOfMassPosition;
			::Eldritch2::float16	centerOfMassVelocity[3];
		};

	// ---

		enum ActiveFoot : bool {
			Left,
			Right
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref BipedalLocomotionAnimationLayer instance.
		BipedalLocomotionAnimationLayer();
		//!	Constructs this @ref BipedalLocomotionAnimationLayer instance.
		BipedalLocomotionAnimationLayer( const Animation::BipedalLocomotionAnimationLayer& ) = delete;

		//!	Destroys this @ref BipedalLocomotionAnimationLayer instance.
		~BipedalLocomotionAnimationLayer() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		FootstepState	_footStates[2];
	};

}	// namespace Animation
}	// namespace Eldritch2