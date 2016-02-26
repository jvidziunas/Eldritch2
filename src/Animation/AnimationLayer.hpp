/*==================================================================*\
  AnimationLayer.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Scripting/ReferenceCountable.hpp>
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		class	KeyframeAnimationResourceView;
		class	KeyCache;
	}
}

namespace Eldritch2 {
namespace Animation {

	class AnimationLayer : public ::Eldritch2::IntrusiveForwardListBaseHook, public Scripting::ReferenceCountable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref AnimationLayer instance.
		AnimationLayer( const AnimationLayer& ) = default;
		//! Constructs this @ref AnimationLayer instance.
		AnimationLayer() = default;

		~AnimationLayer() = default;

	// ---------------------------------------------------

	public:
		virtual void	EvaluateCacheForTime( Animation::KeyCache& keyCache, Animation::BoneIndex maximumBoneToConsider, ::Eldritch2::uint64 time ) abstract;

	// - DATA MEMBERS ------------------------------------

	public:
		static const char* const	scriptTypeName;
	};

}	// namespace Animation
}	// namespace Eldritch2
