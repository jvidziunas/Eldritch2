/*==================================================================*\
  SkeletalArmature.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/ScriptComponents/Armature.hpp>
#include <Physics/PhysX/SmartPointers.hpp>
//------------------------------------------------------------------//
#include <PxArticulationLink.h>
#include <PxArticulation.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}

	namespace Physics {
		namespace PhysX {
			namespace AssetViews {
				class	ArmatureView;
			}
		}
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {

	class SkeletalArmature : public Animation::ScriptComponents::Armature {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class PhysicsAnimationLayer : public Animation::AnimationLayer {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref PhysicsAnimationLayer instance.
			PhysicsAnimationLayer( const AssetViews::ArmatureView& assetView, PhysX::UniquePointer<physx::PxArticulation> articulation );
		//!	Disable copying.
			PhysicsAnimationLayer( const PhysicsAnimationLayer& ) = delete;

			~PhysicsAnimationLayer() = default;

		// ---------------------------------------------------

		public:
			void	EvaluateCacheForAlphaFactor( Animation::KeyCache& keyCache, Animation::BoneIndex maximumBoneToConsider, Eldritch2::uint64 time ) override;

		// ---------------------------------------------------

		//!	Disable assignment.
			PhysicsAnimationLayer&	operator=( const PhysicsAnimationLayer& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
			const AssetViews::ArmatureView&						_assetView;
			const PhysX::UniquePointer<physx::PxArticulation>	_articulation;

		// ---------------------------------------------------

			friend class SkeletalArmature;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SkeletalArmature instance.
		SkeletalArmature( Eldritch2::RigidTransform rootTransform, const AssetViews::ArmatureView& assetView, PhysX::UniquePointer<physx::PxArticulation> articulation, Eldritch2::Allocator& allocator );
	//!	Disable copying.
		SkeletalArmature( const SkeletalArmature& ) = delete;

		~SkeletalArmature();

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref SkeletalArmature type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& typeRegistrar );

	// ---------------------------------------------------
		
	public:
		const PhysicsAnimationLayer&	GetPhysicsAnimationLayer() const;
		PhysicsAnimationLayer&			GetPhysicsAnimationLayer();

	// ---------------------------------------------------

	public:
		const physx::PxArticulation&	GetArticulation() const;
		physx::PxArticulation&			GetArticulation();

	// - DATA MEMBERS ------------------------------------

	private:
		PhysicsAnimationLayer	_physicsAnimationLayer;
	};

} // namespace ScriptComponents
} // namespace PhysX
} // namespace Physics
} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/ScriptComponents/SkeletalArmature.inl>
//------------------------------------------------------------------//