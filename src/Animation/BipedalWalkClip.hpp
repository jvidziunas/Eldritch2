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
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	class BipedalWalkClip : public Animation::Clip {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct FootState {
			Vector          centerOfMassPosition;
			float32         centerOfMassVelocity[3];
			MicrosecondTime strikeTime;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref BipedalWalkClip instance.
		BipedalWalkClip(MicrosecondTime startTime) ETNoexceptHint;
		//!	Constructs this @ref BipedalWalkClip instance.
		BipedalWalkClip(const BipedalWalkClip&) ETNoexceptHint = default;

		~BipedalWalkClip() = default;

		// - DATA MEMBERS ------------------------------------

	private:
		FootState _feet[2];
	};

}} // namespace Eldritch2::Animation
