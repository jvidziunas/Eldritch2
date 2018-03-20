/*==================================================================*\
  AnimationTree.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	template <typename Clip, typename... ConstructorArguments>
	ETInlineHint Result<Clip*> AnimationTree::AddClip( ConstructorArguments&&... arguments ) {
		auto clip( CreateUnique<Clip>( _allocator, eastl::forward<ConstructorArguments>( arguments )... ) );
		if (!clip) {
			return Error::OutOfMemory;
		}

		_clips.EmplaceBack( eastl::move( clip ) );

		return _clips.Back().Get();
	}

// ---------------------------------------------------

	template <typename Blend, typename... ConstructorArguments>
	ETInlineHint Result<Blend*> AnimationTree::AddBlend( ConstructorArguments&&... arguments ) {
		auto blend( CreateUnique<Blend>( _allocator, eastl::forward<ConstructorArguments>( arguments )... ) );
		if (!blend) {
			return Error::OutOfMemory;
		}

		_blends.EmplaceBack( eastl::move( blend ) );

		return _blends.Back().Get();
	}

// ---------------------------------------------------

	ETInlineHint void AnimationTree::SetRoot( const Blend& root ) {
		_root = &root;
	}

}	// namespace Animation
}	// namespace Eldritch2

