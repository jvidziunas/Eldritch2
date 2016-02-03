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
#include <Scripting/ReferenceCountable.hpp>
#include <Physics/PhysX/SmartPointers.hpp>
#include <Foundation/WorldView.hpp>
//------------------------------------------------------------------//
#include <PxSimulationEventCallback.h>
#include <PxContactModifyCallback.h>
#include <PxFiltering.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}
}

namespace physx {
	class	PxControllerManager;
	class	PxController;
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

	// ---------------------------------------------------

		void	onContactModify( ::physx::PxContactModifyPair* const pairs, ::physx::PxU32 count ) override;

	// ---------------------------------------------------

		static ::physx::PxFilterFlags	FilterShader( ::physx::PxFilterObjectAttributes attributes0, ::physx::PxFilterData filterData0,
													  ::physx::PxFilterObjectAttributes attributes1, ::physx::PxFilterData filterData1,
													  ::physx::PxPairFlags& pairFlags,
													  const void* constantBlock, ::physx::PxU32 constantBlockSize );

	// ---------------------------------------------------

	protected:
		static WorldView&	GetActiveWorldView();

	// - TYPE PUBLISHING ---------------------------------

		class CharacterControllerComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref CharacterControllerComponent instance.
			CharacterControllerComponent( PhysX::UniquePointer<::physx::PxController>&& controller );

			//!	Destroys this @ref CharacterControllerComponent instance.
			~CharacterControllerComponent();

		// ---------------------------------------------------

			void	SetEnabled( bool value = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref CharacterControllerComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const					scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxController>	_controller;
		};

	// ---

		class ArticulatedBodyComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref ArticulatedBodyComponent.
			ArticulatedBodyComponent( PhysX::UniquePointer<::physx::PxArticulation>&& articulation );

			//! Constructs this @ref ArticulatedBodyComponent.
			~ArticulatedBodyComponent();

		// ---------------------------------------------------

			void	SetEnabled( bool value = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref ArticulatedBodyComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const						scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxArticulation>	_articulation;
			bool											_enabled;
		};

	// ---

		class TerrainColliderComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref TerrainColliderComponent.
			TerrainColliderComponent( PhysX::UniquePointer<::physx::PxRigidStatic>&& actor );

			//! Constructs this @ref TerrainColliderComponent.
			~TerrainColliderComponent();

		// ---------------------------------------------------

			void	SetEnabled( bool value = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref TerrainColliderComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const						scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxRigidStatic>	_actor;
			bool											_enabled;
		};

	// ---

		class TriggerVolumeComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref TriggerVolumeComponent.
			TriggerVolumeComponent( PhysX::UniquePointer<::physx::PxRigidStatic>&& actor );

			//!	Constructs this @ref TriggerVolumeComponent.
			~TriggerVolumeComponent();

		// ---------------------------------------------------

			void	SetEnabled( bool value = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref TriggerVolumeComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const						scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxRigidStatic>	_actor;
			bool											_enabled;
		};

	// ---

		class PhysicalSoftBodyComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PhysicalSoftBodyComponent.
			PhysicalSoftBodyComponent( PhysX::UniquePointer<::physx::PxCloth>&& actor );

			//!	Constructs this @ref PhysicalSoftBodyComponent.
			~PhysicalSoftBodyComponent();

		// ---------------------------------------------------

			void	SetEnabled( bool value = true );

			bool	GetEnabled() const;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref PhysicalSoftBodyComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const				scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxCloth>	_actor;
			bool									_enabled;
		};

	// ---------------------------------------------------

		ETInlineHint const ::physx::PxControllerManager&	GetControllerManager() const;
		ETInlineHint ::physx::PxControllerManager&			GetControllerManager();

		ETInlineHint const ::physx::PxScene&				GetScene() const;
		ETInlineHint ::physx::PxScene&						GetScene();

	// - DATA MEMBERS ------------------------------------

	private:
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