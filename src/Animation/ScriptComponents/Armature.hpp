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
#include <Utility/Containers/IntrusiveList.hpp>
#include <Animation/AnimationLayer.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Animation/KeyCache.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}
}

namespace Eldritch2 {
namespace Animation {
namespace ScriptComponents {

	class Armature {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//! Constructs this @ref Armature instance.
	/*!	@remarks Designed to be called by subclasses. */
		Armature( Eldritch2::RigidTransform rootTransform, Eldritch2::Allocator& allocator );
	//! Disable copying.
		Armature( const Armature& ) = delete;

	//! Destroys this @ref Armature instance.
	/*!	@remarks Designed to be called by subclasses. */
		~Armature() = default;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref Armature type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// ---------------------------------------------------

	public:
		Eldritch2::RigidTransform ETSimdCall	GetRootTransform() const;

	// ---------------------------------------------------

	public:
		const Animation::KeyCache&	GetKeyCache() const;
		Animation::KeyCache&		GetKeyCache();

	// ---------------------------------------------------

	public:
		void	AttachLayer( Animation::AnimationLayer& layer );

		void	AttachChildLayer( Animation::AnimationLayer& layer, Animation::AnimationLayer& parent );

		void	DetachLayer( Animation::AnimationLayer& layer );

	// ---------------------------------------------------

	public:
		void	UpdateCachedKeys( Animation::BoneIndex maximumBoneToConsider, Eldritch2::float16 alpha );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::RigidTransform							_rootTransform;
		Animation::KeyCache									_keyCache;
		Eldritch2::IntrusiveList<Animation::AnimationLayer>	_layers;
	};

}	// namespace ScriptComponents
}	// namespace Animation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Animation/ScriptComponents/Armature.inl>
//------------------------------------------------------------------//