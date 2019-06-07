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
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	class AnimationTree {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct RootLink {
			AnimationTree* parent;
			BoneIndex      bone;
		};
		
		// ---

	public:
		using ClipList = ArrayList<UniquePointer<Clip>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		AnimationTree(const AnimationTree&) = delete;
		//! Constructs this @ref AnimationTree instance.
		AnimationTree() ETNoexceptHint;

		~AnimationTree() = default;

		// ---------------------------------------------------

	public:
		template <typename PublicClip, typename... CtorArgs>
		PublicClip& InsertClip(CtorArgs&&...);

		// ---------------------------------------------------

	public:
		Result BindResources(const ArmatureDefinition& definition);

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AnimationTree& operator=(const AnimationTree&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const ArmatureDefinition* _definition;
		MallocAllocator           _allocator;
		ClipList                  _clips;

		// ---------------------------------------------------

		friend void Swap(AnimationTree&, AnimationTree&) ETNoexceptHint;
	};

}} // namespace Eldritch2::Animation

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AnimationTree.inl>
//------------------------------------------------------------------//
