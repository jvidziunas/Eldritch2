/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/btDiscreteDynamicsMixinWrapper.hpp>
#include <Physics/BulletDynamics/btHeightfieldTerrainShapeEx.hpp>
#include <Physics/BulletDynamics/btSoftBodyWorldMixin.hpp>
#include <Utility/Containers/UnorderedMap.hpp>
#include <Scripting/ReferenceCountable.hpp>
#include <Foundation/WorldView.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <LinearMath/btPoolAllocator.h>
#include <btBulletDynamicsCommon.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
		class	ReferenceCountable;
	}

	namespace Physics {
		namespace BulletDynamics {
			class	EngineService;
		}
	}
}

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	class ET16ByteAligned WorldView : public Foundation::WorldView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using BulletWorld				= ::btSoftBodyWorldMixin<::btDiscreteDynamicsMixinWrapper<>>;
		using CollisionConfiguration	= BulletWorld::CollisionConfiguration;
		using CollisionDispatcher		= ::btCollisionDispatcher;
		using OverlappingPairCache		= ::btHashedOverlappingPairCache;
		using BroadphaseInterface		= ::bt32BitAxisSweep3;
		using ConstraintSolver			= BulletWorld::ConstraintSolver;
		using SoftBodySolver			= ::btDefaultSoftBodySolver;

	// ---

		enum FilterBehaviors : short {
			DefaultFilter		= 1,
			StaticFilter		= 2,
			KinematicFilter		= 4,
			DebrisFilter		= 8,
			SensorTriggerFilter	= 16,
			CharacterFilter		= 32,
			VisibilityFilter	= 64,
			ProjectileFilter	= 128,
			CustomGroupBegin	= 256,
			None				= 0,
			All					= ~None,
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld, const BulletDynamics::EngineService& hostingEngine );

		//!	Destroys this @ref WorldView instance.
		~WorldView() = default;

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		void	OnPreScriptTick( Scheduler::WorkerContext& executingContext ) override sealed;

		void	OnPostScriptTick( Scheduler::WorkerContext& executingContext ) override sealed;

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;

	// ---------------------------------------------------

		static WorldView&	GetActiveWorldView();

	// - TYPE PUBLISHING ---------------------------------

		class CharacterArmatureComponent : public Animation::Armature {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				CollisionFilterGroup	= (FilterBehaviors::CharacterFilter),
				CollisionFilterMask		= (FilterBehaviors::StaticFilter | FilterBehaviors::DefaultFilter | FilterBehaviors::SensorTriggerFilter)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref CharacterArmatureComponent instance.
			CharacterArmatureComponent( WorldView& owningWorldView = GetActiveWorldView() );

			//!	Destroys this @ref CharacterArmatureComponent instance.
			~CharacterArmatureComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref CharacterArmatureComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const	scriptTypeName;

		private:
			::btPairCachingGhostObject			_ghostObject;
			::btKinematicCharacterController	_controller;
		};

	// ---

		class TerrainColliderComponent : public Scripting::ReferenceCountable {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				CollisionFilterGroup	= FilterBehaviors::StaticFilter,
				CollisionFilterMask		= FilterBehaviors::None
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref TerrainColliderComponent.
			TerrainColliderComponent( WorldView& owningView = GetActiveWorldView() );

			//! Constructs this @ref TerrainColliderComponent.
			~TerrainColliderComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//!	Registers all script-callable methods for the @ref TerrainColliderComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const		scriptTypeName;

		private:
			::btHeightfieldTerrainShapeEx	_collisionShape;
			::btRigidBody					_body;
		};

	// ---

		class TriggerVolumeComponent : public Scripting::ReferenceCountable {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				CollisionFilterGroup	= FilterBehaviors::SensorTriggerFilter,
				CollisionFilterMask		= FilterBehaviors::None
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref TriggerVolumeComponent.
			TriggerVolumeComponent( WorldView& owningView = GetActiveWorldView() );

			//!	Constructs this @ref TriggerVolumeComponent.
			~TriggerVolumeComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
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
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				CollisionFilterGroup	= FilterBehaviors::DefaultFilter,
				CollisionFilterMask		= (FilterBehaviors::StaticFilter | FilterBehaviors::DefaultFilter)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref PhysicalSoftBodyComponent.
			PhysicalSoftBodyComponent( WorldView& owningView = GetActiveWorldView() );

			//!	Constructs this @ref PhysicalSoftBodyComponent.
			~PhysicalSoftBodyComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
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

	// - DATA MEMBERS ------------------------------------

	private:
		::btPoolAllocator		_persistentManifoldPool;
		::btPoolAllocator		_collisionAlgorithmPool;
		SoftBodySolver			_softBodySolver;
		CollisionConfiguration	_collisionConfiguration;
		CollisionDispatcher		_dispatcher;
		OverlappingPairCache	_pairCache;
		BroadphaseInterface		_broadphaseInterface;
		ConstraintSolver		_constraintSolver;
		BulletWorld				_dynamicsWorld;
		::btGhostPairCallback	_ghostPairCallback;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2