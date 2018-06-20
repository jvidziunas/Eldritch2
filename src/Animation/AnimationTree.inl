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

		template <typename PublicBlend, typename... ConstructorArguments>
		ETInlineHint void AnimationTree::AddBlend(ConstructorArguments&&... arguments) {
			void* const memory(_allocator.Allocate(sizeof(PublicBlend), alignof(PublicBlend), 0u));
			if (memory == nullptr) {
				return;
			}

			_blends.EmplaceBack(new(memory) PublicBlend(eastl::forward<ConstructorArguments>(arguments)...));
		}

	// ---------------------------------------------------

		template <typename PublicClip, typename... ConstructorArguments>
		ETInlineHint void AnimationTree::AddClip(ConstructorArguments&&... arguments) {
			void* const memory(_allocator.Allocate(sizeof(PublicClip), alignof(PublicClip), 0u));
			if (memory == nullptr) {
				return;
			}

			_clips.EmplaceBack(new(memory) PublicClip(eastl::forward<ConstructorArguments>(arguments)...));
		}

	// ---------------------------------------------------

		ETInlineHint void AnimationTree::SetRoot(const Blend& root) {
			_root = &root;
		}

	}	// namespace Animation
}	// namespace Eldritch2

