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

	template <typename Allocator>
	AnimationTree<Allocator>::AnimationTree(const AllocatorType& allocator) :
		_allocator(allocator),
		_clips(ChildAllocator(_allocator, "Animation Tree Clip List Allocator")),
		_blends(ChildAllocator(_allocator, "Animation Tree Blend List Allocator")) {
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint const KnotCache& AnimationTree<Allocator>::GetKnots() const {
		return _knots;
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint const Blend* AnimationTree<Allocator>::GetRoot() const {
		return _blends[_root].Get();
	}

	// ---------------------------------------------------

	template <typename Allocator>
	template <typename PublicBlend, typename... ConstructorArguments>
	ETInlineHint PublicBlend& AnimationTree<Allocator>::InsertBlend(ConstructorArguments&&... arguments) {
		_blends.Append(MakeUnique<PublicBlend>(_allocator, eastl::forward<ConstructorArguments>(arguments)...));
		return *static_cast<PublicBlend*>(_blends.Back().Get());
	}

	// ---------------------------------------------------

	template <typename Allocator>
	template <typename PublicClip, typename... ConstructorArguments>
	ETInlineHint PublicClip& AnimationTree<Allocator>::InsertClip(ConstructorArguments&&... arguments) {
		_clips.Append(MakeUnique<PublicClip>(_allocator, eastl::forward<ConstructorArguments>(arguments)...));
		return *static_cast<PublicClip*>(_clips.Back().Get());
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void AnimationTree<Allocator>::SetRoot(uint32 index) {
		_root = index;
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void AnimationTree<Allocator>::FetchKnots(uint64 time, BoneIndex maximumBone) {
		for (const UniquePointer<Clip>& clip : _clips) {
			clip->FetchKnots(_knots, time, maximumBone);
		}
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void AnimationTree<Allocator>::Clear() {
		_blends.Clear(ReleaseMemorySemantics());
		_clips.Clear(ReleaseMemorySemantics());
		_root = 0u;
	}

	// ---------------------------------------------------

	template <typename Allocator>
	ETInlineHint void Swap(AnimationTree<Allocator>& lhs, AnimationTree<Allocator>& rhs) {
		using ::Eldritch2::Swap;

		Swap(lhs._allocator, rhs._allocator);
		Swap(lhs._knots, rhs._knots);
		Swap(lhs._clips, rhs._clips);
		Swap(lhs._blends, rhs._blends);
		Swap(lhs._root, rhs._root);
	}

}} // namespace Eldritch2::Animation
