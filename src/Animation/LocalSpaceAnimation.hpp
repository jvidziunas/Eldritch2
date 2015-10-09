/*==================================================================*\
  LocalSpaceAnimation.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {
		struct	Transform;
	}
}

namespace Eldritch2 {
namespace Animation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Animation		= ::Eldritch2::Animation;

// ---------------------------------------------------

	class LocalSpaceAnimation {
	public:
		struct AnimationKey {
			::Eldritch2::uint32	_affectedTransform;
		};

		LocalSpaceAnimation( ::Eldritch2::uint32 keyCount );

	// ---------------------------------------------------

		void	ApplyAdditive( const ::Eldritch2::Range<Animation::Transform*> affectedTransforms, ::Eldritch2::float32 weight ) const;

	// - DATA MEMBERS ------------------------------------

	private:
		const ::Eldritch2::uint32	_keyCount;
		AnimationKey				_keys[];
	};

}	// namespace Animation
}	// namespace Eldritch2