/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ObjectPoolAllocator.hpp>
#include <Scripting/ReferenceCountable.hpp>
#include <Physics/PhysX/SmartPointers.hpp>
#include <Foundation/WorldView.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <PxSimulationEventCallback.h>
#include <PxContactModifyCallback.h>
#include <PxFiltering.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptApiRegistrationInitializationVisitor;
	}

	namespace Physics {
		namespace PhysX {
			class	TerrainResourceView;
			class	MeshResourceView;
		}
	}
}

namespace physx {
	class	PxControllerManager;
	class	PxCapsuleController;
	class	PxScene;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class WorldView : public Foundation::WorldView, public ::physx::PxSimulationEventCallback, public ::physx::PxContactModifyCallback {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		/*!	@param[in] scene PhysX scene the new @ref WorldView will be responsible for controlling.
			@param[in] controllerManager PhysX controller manager the new @ref WorldView will use to create character controller instances.
			@param[in] world @ref World that controls the lifetime of the @ref WorldView.
			*/
		WorldView( PhysX::UniquePointer<::physx::PxScene>&& scene, PhysX::UniquePointer<::physx::PxControllerManager>&& controllerManager, Foundation::World& world );

		~WorldView();

	// ---------------------------------------------------

		void	OnPreScriptTick( Scheduler::WorkerContext& executingContext ) override;

		void	OnScriptTick( Scheduler::WorkerContext& executingContext ) override;

		void	OnPostScriptTick( Scheduler::WorkerContext& executingContext ) override;

	// - PXSIMULATIONEVENTCALLBACK METHODS ---------------

		void	onConstraintBreak( ::physx::PxConstraintInfo* constraints, ::physx::PxU32 count ) override;

		void	onWake( ::physx::PxActor** actors, ::physx::PxU32 count ) override;

		void	onSleep( ::physx::PxActor** actors, ::physx::PxU32 count ) override;

		void	onContact( const ::physx::PxContactPairHeader& pairHeader, const ::physx::PxContactPair* pairs, ::physx::PxU32 nbPairs ) override;

		void	onTrigger( ::physx::PxTriggerPair* pairs, ::physx::PxU32 count ) override;

	// - PXCONTACTMODIFYCALLBACK METHODS -----------------

		void	onContactModify( ::physx::PxContactModifyPair* const pairs, ::physx::PxU32 count ) override;

	// ---------------------------------------------------

		static ::physx::PxFilterFlags	FilterShader( ::physx::PxFilterObjectAttributes attributes0, ::physx::PxFilterData filterData0,
													  ::physx::PxFilterObjectAttributes attributes1, ::physx::PxFilterData filterData1,
													  ::physx::PxPairFlags& pairFlags,
													  const void* constantBlock, ::physx::PxU32 constantBlockSize );

	// ---------------------------------------------------

		static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

	// ---------------------------------------------------

	protected:
		static ETNoAliasHint ::Eldritch2::float32	GetTickDurationInSeconds();

		static ETNoAliasHint size_t					GetComponentPoolElementSizeInBytes();

		static ETNoAliasHint size_t					GetArmaturePoolElementSizeInBytes();

	// ---------------------------------------------------

		static WorldView&	GetActiveWorldView();

	// - TYPE PUBLISHING ---------------------------------

		class PhysicalArmature : public Animation::Armature {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class PhysicsAnimationLayer : public Animation::AnimationLayer {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				//!	Constructs this @ref PhysicsAnimationLayer instance.
				PhysicsAnimationLayer( PhysX::UniquePointer<::physx::PxAggregate>&& aggregate );

				~PhysicsAnimationLayer();

			// ---------------------------------------------------

				ETInlineHint ::physx::PxAggregate&	GetAggregate();

			// ---------------------------------------------------

				void	EvaluateCacheForTime( Animation::KeyCache& keyCache, Animation::BoneIndex maximumBoneToConsider, ::Eldritch2::uint64 time ) override;

			// ---------------------------------------------------

				void	Dispose() override;

			// - DATA MEMBERS ------------------------------------

			private:
				PhysX::UniquePointer<::physx::PxAggregate>	_aggregate;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref PhysicalArmature instance.
			PhysicalArmature( PhysX::UniquePointer<::physx::PxAggregate>&& aggregate );

			~PhysicalArmature();

		// ---------------------------------------------------

			ETInlineHint ::physx::PxAggregate&	GetAggregate();

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref PhysicalArmature type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const	scriptTypeName;

		private:
			PhysicsAnimationLayer		_physicsAnimationLayer;
		};

	// ---

		class AnimationDrivenCharacterCollider : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref AnimationDrivenCharacterCollider instance.
			AnimationDrivenCharacterCollider( PhysX::UniquePointer<::physx::PxCapsuleController>&& controller );

			//!	Destroys this @ref AnimationDrivenCharacterCollider instance.
			~AnimationDrivenCharacterCollider();

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref AnimationDrivenCharacterCollider type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const							scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxCapsuleController>	_controller;
		};

	// ---

		class MeshCollider : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref MeshCollider.
			MeshCollider( PhysicalArmature& armature, const PhysX::MeshResourceView& asset );

			//! Constructs this @ref MeshCollider.
			~MeshCollider();

		// ---------------------------------------------------

			void	SetEnabled( bool enabled = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref MeshCollider type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const					scriptTypeName;

		private:
			Scripting::ObjectHandle<PhysicalArmature>	_armature;
			const PhysX::MeshResourceView*				_asset;
			bool										_enabled;
		};

	// ---

		class TerrainCollider : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref TerrainCollider.
			TerrainCollider( PhysX::UniquePointer<::physx::PxRigidStatic>&& actor, const PhysX::TerrainResourceView& asset );

			//! Constructs this @ref TerrainCollider.
			~TerrainCollider();

		// ---------------------------------------------------

			void	SetEnabled( bool enabled = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref TerrainCollider type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const						scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxRigidStatic>	_actor;
			const PhysX::TerrainResourceView*				_asset;
			bool											_enabled;
		};

	// ---

		class TriggerVolume : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref TriggerVolume.
			TriggerVolume( PhysX::UniquePointer<::physx::PxRigidStatic>&& actor );

			//!	Constructs this @ref TriggerVolume.
			~TriggerVolume();

		// ---------------------------------------------------

			void	SetEnabled( bool value = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref TriggerVolume type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptApiRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const						scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxRigidStatic>	_actor;
			bool											_enabled;
		};

	// ---------------------------------------------------

		ETInlineHint const ::physx::PxControllerManager&	GetControllerManager() const;
		ETInlineHint ::physx::PxControllerManager&			GetControllerManager();

		ETInlineHint const ::physx::PxScene&				GetScene() const;
		ETInlineHint ::physx::PxScene&						GetScene();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ObjectPoolAllocator					_armatureAllocator;
		::Eldritch2::ObjectPoolAllocator					_componentAllocator;
		PhysX::UniquePointer<::physx::PxScene>				_scene;
		PhysX::UniquePointer<::physx::PxControllerManager>	_controllerManager;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/WorldView.inl>
//------------------------------------------------------------------//