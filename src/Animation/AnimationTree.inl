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

namespace Eldritch2 { namespace Animation {

	ETInlineHint ETForceInlineHint AnimationTree::AnimationTree() ETNoexceptHint : _definition(nullptr), _allocator("Animation Tree Root Allocator"), _clips(MallocAllocator("Animation Tree Clip List Allocator")) {}

	// ---------------------------------------------------

	template <typename PublicClip, typename... CtorArgs>
	ETInlineHint ETForceInlineHint PublicClip& AnimationTree::InsertClip(CtorArgs&&... ctorArgs) {
		return *_clips.EmplaceBack(MakeUnique<PublicClip>(_allocator, *this, Forward<CtorArgs>(ctorArgs)...));
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint ETForceInlineHint void Swap(AnimationTree& lhs, AnimationTree& rhs) ETNoexceptHint {
		using ::Eldritch2::Swap;

		Swap(lhs._definition, rhs._definition);
		Swap(lhs._allocator, rhs._allocator);
		Swap(lhs._clips, rhs._clips);
	}

}} // namespace Eldritch2::Animation
