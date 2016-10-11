/*==================================================================*\
  KeyCache.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Animation {

	class KeyCache {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref KeyCache instance.
		KeyCache( Eldritch2::Allocator& allocator );

		~KeyCache() = default;

	// ---------------------------------------------------

		void	AddKey( const Animation::BoneAnimationKey& key );

	// ---------------------------------------------------

		bool	IsValidForAlphaFactor( Eldritch2::float16 time ) const;

		void	Invalidate();

	// ---------------------------------------------------

		Eldritch2::Range<const BoneAnimationKey*>	GetCachedKeys() const;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::float16										_minValidTime;
		Eldritch2::float16										_maxValidTime;
		Eldritch2::ResizableArray<Animation::BoneAnimationKey>	_keys;
	};

}	// namespace Animation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/KeyCache.inl>
//------------------------------------------------------------------//