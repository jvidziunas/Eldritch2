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
			Vector  centerOfMassPosition;
			float16 centerOfMassVelocity[3];
			uint64  stepEndTime;
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
		FootState _feet[FootCount];
	};

}} // namespace Eldritch2::Animation
