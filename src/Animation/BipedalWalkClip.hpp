/*==================================================================*\
  BipedalWalkClip.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	class BipedalWalkClip : public Animation::Clip {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct FootstepState {
			uint64  endTime;
			Vector  centerOfMassPosition;
			float16 centerOfMassVelocity[3];
		};

		// ---

	public:
		enum : size_t {
			FootCount = 2u
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref BipedalWalkClip instance.
		BipedalWalkClip(const BipedalWalkClip&) = delete;
		//!	Constructs this @ref BipedalWalkClip instance.
		BipedalWalkClip();

		~BipedalWalkClip() = default;

		// - DATA MEMBERS ------------------------------------

	private:
		FootstepState _footStates[FootCount];
	};

}} // namespace Eldritch2::Animation
