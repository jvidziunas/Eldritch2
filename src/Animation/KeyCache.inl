/*==================================================================*\
  KeyCache.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Containers/Range.hpp>
//------------------------------------------------------------------//
#include <eastl/numeric_limits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	ETInlineHint void KeyCache::AddKey( const Animation::BoneAnimationKey& key ) {
		_maxValidTime	= Eldritch2::Min( _maxValidTime, key.destinationKnot.time );
		_minValidTime	= Eldritch2::Max( _minValidTime, key.sourceKnot.time );

		_keys.PushBack( key );
	}

// ---------------------------------------------------

	ETInlineHint bool KeyCache::IsValidForAlphaFactor( Eldritch2::float16 alpha ) const {
		return static_cast<bool>( ( _minValidTime <= alpha ) & ( alpha <= _maxValidTime ) );
	}

// ---------------------------------------------------

	ETInlineHint void KeyCache::Invalidate() {
		_keys.Clear();

		_maxValidTime = eastl::numeric_limits<decltype( _maxValidTime )>::min();
		_minValidTime = eastl::numeric_limits<decltype( _minValidTime )>::max();
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Range<const Animation::BoneAnimationKey*> KeyCache::GetCachedKeys() const {
		return { _keys.Begin(), _keys.End() };
	}

}	// namespace Animation
}	// namespace Eldritch2