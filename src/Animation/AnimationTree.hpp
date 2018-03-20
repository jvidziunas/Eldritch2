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
#include <Animation/Blend.hpp>
#include <Animation/Clip.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}

	class	Transformation;
	template <typename>
	class	Result;
}

namespace ispc {
	struct	GpuTransformWithVelocity;
	struct	GpuTransform;
}

namespace Eldritch2 {
namespace Animation {

	class AnimationTree {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Disable copy construction.
		AnimationTree( const AnimationTree& ) = delete;
	//! Constructs this @ref AnimationTree instance.
		AnimationTree();

		~AnimationTree() = default;

	// ---------------------------------------------------

	public:
		void	PrefetchClipTransforms( Scheduling::JobExecutor& executor, uint64 worldTimeBegin, uint64 worldTimeEnd );

		void	EvaluatePose( Transformation localToWorld, BoneIndex maximumBoneToAnimate, ispc::GpuTransformWithVelocity transforms[] ) const;
		void	EvaluatePose( Transformation localToWorld, BoneIndex maximumBoneToAnimate, ispc::GpuTransform transforms[] ) const;

	// ---------------------------------------------------

	public:
		template <typename Clip, typename... ConstructorArguments>
		Result<Clip*>	AddClip( ConstructorArguments&&... arguments );

		template <typename Blend, typename... ConstructorArguments>
		Result<Blend*>	AddBlend( ConstructorArguments&&... arguments );

		void			SetRoot( const Blend& root );

	// ---------------------------------------------------

	public:
		void	Reset();

	// - DATA MEMBERS ------------------------------------

	private:
		mutable ArenaChildAllocator		_allocator;
	//	To consider: Can we sort clips/blends for better cache behavior?
	//!	Collection of all animation data sources feeding this @ref AnimationTree.
		ArrayList<UniquePointer<Clip>>	_clips;
	//!	Collection of @ref Blend objects describing how to combine @ref Clip instances to form the final pose.
		ArrayList<UniquePointer<Blend>>	_blends;
	//!	Root @ref Blend for the tree. The pose will be evaluated starting from this element.
		const Blend*					_root;
	};

}	// namespace Animation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/AnimationTree.inl>
//------------------------------------------------------------------//