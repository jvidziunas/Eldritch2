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
		enum : size_t {
			FootCount	= 2u
		};

	// ---

	public:
		struct FootstepState {
			Eldritch2::uint64	endTime;
			Eldritch2::Float4	centerOfMassPosition;
			Eldritch2::float16	centerOfMassVelocity[3];
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref BipedalLocomotionAnimationLayer instance.
		BipedalLocomotionAnimationLayer();
	//!	Constructs this @ref BipedalLocomotionAnimationLayer instance.
		BipedalLocomotionAnimationLayer( const BipedalLocomotionAnimationLayer& ) = delete;

		~BipedalLocomotionAnimationLayer() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		FootstepState	_footStates[FootCount];
	};

}	// namespace Animation
}	// namespace Eldritch2