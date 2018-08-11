/*==================================================================*\
  AnimationTree.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	template <typename Allocator = MallocAllocator>
	class AnimationTree {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using AllocatorType = Allocator;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AnimationTree instance.
		AnimationTree(const AllocatorType& allocator);
		//! Disable copy construction.
		AnimationTree(const AnimationTree&) = delete;

		~AnimationTree() = default;

		// ---------------------------------------------------

	public:
		const KnotCache& GetKnots() const;

		const Blend* GetRoot() const;

		// ---------------------------------------------------

	public:
		void FetchKnots(uint64 time, BoneIndex maximumBone);

		// ---------------------------------------------------

	public:
		template <typename PublicBlend, typename... ConstructorArguments>
		PublicBlend& InsertBlend(ConstructorArguments&&... arguments);

		template <typename PublicClip, typename... ConstructorArguments>
		PublicClip& InsertClip(ConstructorArguments&&... arguments);

		void SetRoot(uint32 index);

		// ---------------------------------------------------

	public:
		void Clear();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AnimationTree& operator=(const AnimationTree&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Allocator                               _allocator;
		KnotCache                                       _knots;
		ArrayList<UniquePointer<Clip>, ChildAllocator>  _clips;
		ArrayList<UniquePointer<Blend>, ChildAllocator> _blends;
		uint32                                          _root;

		// ---------------------------------------------------

		template <typename Allocator>
		friend void Swap(AnimationTree<Allocator>&, AnimationTree<Allocator>&);
	};

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AnimationTree.inl>
//------------------------------------------------------------------//
