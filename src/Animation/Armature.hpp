/*==================================================================*\
  Armature.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Scripting/ReferenceCountable.hpp>
#include <Animation/AnimationLayer.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Animation/KeyCache.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptApiRegistrationInitializationVisitor;
	}
}

namespace Eldritch2 {
namespace Animation {

	class Armature : public Scripting::ReferenceCountable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref Armature instance.
		/*!	@remarks Designed to be called by subclasses.
			*/
		Armature( const Animation::Armature& ) = delete;
		//! Constructs this @ref Armature instance.
		/*!	@remarks Designed to be called by subclasses.
			*/
		Armature() = default;

		//! Destroys this @ref Armature instance.
		/*!	@remarks Designed to be called by subclasses.
			*/
		~Armature() = default;

	// ---------------------------------------------------

	public:
		ETInlineHint const Animation::KeyCache&	GetKeyCache() const;

	// ---------------------------------------------------

		ETInlineHint void	AttachLayer( Animation::AnimationLayer& layer );

		ETInlineHint void	AttachChildLayer( Animation::AnimationLayer& layer, Animation::AnimationLayer& parent );

		ETInlineHint void	DetachLayer( Animation::AnimationLayer& layer );

	// ---------------------------------------------------

		void	UpdateCachedKeys( Animation::BoneIndex maximumBoneToConsider, ::Eldritch2::uint64 time );

	// - SCRIPT API REFERENCE ----------------------------

		//! Registers all script-callable methods for the @ref Armature type with the specified script type registrar.
		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& visitor );

	// - DATA MEMBERS ------------------------------------

	public:
		static const char* const										scriptTypeName;

	private:
		Animation::KeyCache												_keyCache;
		::Eldritch2::IntrusiveForwardList<Animation::AnimationLayer>	_layers;
	};

}	// namespace Animation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/Armature.inl>
//------------------------------------------------------------------//