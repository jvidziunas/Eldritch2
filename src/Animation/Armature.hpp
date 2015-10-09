/*==================================================================*\
  Armature.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Utility/Containers/ResizableArray.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Utility/MPL/FloatTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}

	class	ErrorCode;
	class	Float4;
}

namespace Eldritch2 {
namespace Animation {

	namespace Scripting	= ::Eldritch2::Scripting;
	namespace Animation	= ::Eldritch2::Animation;

// ---------------------------------------------------

	class Armature : public Scripting::ReferenceTypeBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class AnimationLayer : public Scripting::ReferenceTypeBase {
		public:
			virtual void	EvaluatePoseForTime( const ::Eldritch2::uint16 maximumBoneToConsider, const ::Eldritch2::uint64 time ) abstract;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//! Constructs this @ref AnimationLayer instance.
			AnimationLayer() = default;
		};

	// ---

		class AdditiveKeyframeAnimationLayer : public ::Eldritch2::IntrusiveForwardListBaseHook, public Armature::AnimationLayer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//! Constructs this @ref AdditiveKeyframeAnimationLayer instance.
			AdditiveKeyframeAnimationLayer() = default;
		};

	// ---

		class ReplacementKeyframeAnimationLayer : public ::Eldritch2::IntrusiveForwardListBaseHook, public Armature::AnimationLayer {
		public:
			virtual ::Eldritch2::Float4	EvaluateRootDisplacementDeltaForTime( const ::Eldritch2::uint64 lastEvalutedTime, const ::Eldritch2::uint64 currentTime ) abstract;

			ETInlineHint void			AttachAnimationLayer( Armature::AdditiveKeyframeAnimationLayer& layer );

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//! Constructs this @ref ReplacementAnimationLayer instance.
			ReplacementKeyframeAnimationLayer() = default;

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::IntrusiveForwardList<Armature::AdditiveKeyframeAnimationLayer>	_attachedAdditiveAnimationLayers;
		};

	// ---

		class PostprocessAnimationLayer : public ::Eldritch2::IntrusiveForwardListBaseHook, public Scripting::ReferenceTypeBase {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//! Constructs this @ref PostprocessAnimationLayer instance.
			PostprocessAnimationLayer() = default;
		};

	// ---------------------------------------------------

	public:
		ETInlineHint ::Eldritch2::Allocator&	GetAllocator();

	// ---------------------------------------------------

		ETInlineHint void	AttachAnimationLayer( Armature::ReplacementKeyframeAnimationLayer& layer );
		ETInlineHint void	AttachAnimationLayer( Armature::PostprocessAnimationLayer& layer );

	// - SCRIPT API REFERENCE ----------------------------

		//! Registers all script-callable methods for the @ref Armature type with the specified script type registrar.
		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this Armature instance.
		Armature( const Animation::Armature& ) = delete;
		//! Constructs this Armature instance.
		Armature() = default;

		//! Destroys this Armature instance.
		~Armature() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		static const char* const	scriptTypeName;

	// ---------------------------------------------------

	protected:
		::Eldritch2::IntrusiveForwardList<ReplacementKeyframeAnimationLayer>	_topLevelAnimations;
		::Eldritch2::IntrusiveForwardList<PostprocessAnimationLayer>			_postprocessAnimations;
	};

}	// namespace Animation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/Armature.inl>
//------------------------------------------------------------------//