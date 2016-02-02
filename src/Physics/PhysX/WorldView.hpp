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
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}
}

namespace physx {
	class	PxCapsuleController;
	class	PxControllerManager;
	class	PxScene;
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class WorldView : public Foundation::WorldView, public ::physx::PxSimulationEventCallback {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		/*!	@param[in] scene PhysX scene the new @ref WorldView will be responsible for controlling.
			@param[in] world @ref World that controls the lifetime of the @ref WorldView.
			*/
		WorldView( PhysX::UniquePointer<::physx::PxScene>&& scene, Foundation::World& world );

		~WorldView();

	// ---------------------------------------------------

		void	OnScriptTick( Scheduler::WorkerContext& executingContext ) override;

		void	OnPostScriptTick( Scheduler::WorkerContext& executingContext ) override;

	// ---------------------------------------------------

		void	onConstraintBreak( ::physx::PxConstraintInfo* constraints, ::physx::PxU32 count ) override;

		void	onWake( ::physx::PxActor** actors, ::physx::PxU32 count ) override;

		void	onSleep( ::physx::PxActor** actors, ::physx::PxU32 count ) override;

		void	onContact( const ::physx::PxContactPairHeader& pairHeader, const ::physx::PxContactPair* pairs, ::physx::PxU32 nbPairs ) override;

		void	onTrigger( ::physx::PxTriggerPair* pairs, ::physx::PxU32 count ) override;

	// - TYPE PUBLISHING ---------------------------------

	protected:
		class CharacterControllerComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref CharacterControllerComponent instance.
			CharacterControllerComponent( PhysX::UniquePointer<::physx::PxCapsuleController>&& controller );

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

		public:
			static const char* const	scriptTypeName;

		// ---------------------------------------------------

		private:
			PhysX::UniquePointer<::physx::PxCapsuleController>	_controller;
			bool												_enabled;
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
			TerrainColliderComponent( PhysX::UniquePointer<::physx::PxActor>&& );

			//! Constructs this @ref TerrainColliderComponent.
			~TerrainColliderComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref TerrainColliderComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

			static const char* const		scriptTypeName;

		private:
			PhysX::UniquePointer<::physx::PxActor>	_actor;
		};

	// ---

#if 0
		class TriggerVolumeComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref TriggerVolumeComponent.
			TriggerVolumeComponent( WorldView& owningView = GetActiveWorldView() );

			//!	Constructs this @ref TriggerVolumeComponent.
			~TriggerVolumeComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref TriggerVolumeComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const	scriptTypeName;

		private:
			::btPairCachingGhostObject	_ghostObject;
		};

	// ---

		class PhysicalSoftBodyComponent : public Scripting::ReferenceCountable {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PhysicalSoftBodyComponent.
			PhysicalSoftBodyComponent( WorldView& owningView = GetActiveWorldView() );

			//!	Constructs this @ref PhysicalSoftBodyComponent.
			~PhysicalSoftBodyComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

			//!	Registers all script-callable methods for the @ref PhysicalSoftBodyComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const	scriptTypeName;

		private:
			::btSoftBody				_body;
		};
#endif

	// - DATA MEMBERS ------------------------------------

	private:
		PhysX::UniquePointer<::physx::PxScene>				_scene;
		PhysX::UniquePointer<::physx::PxControllerManager>	_controllerManager;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2