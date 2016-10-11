/*==================================================================*\
  WorldService.hpp
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
#include <Physics/PhysX/SmartPointers.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Core/WorldService.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxControllerManager.h>
#include <PxSimulationEventCallback.h>
#include <PxContactModifyCallback.h>
#include <PxFiltering.h>
#include <PxScene.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class WorldService : public Core::WorldService, public physx::PxSimulationEventCallback, public physx::PxContactModifyCallback {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref WorldService instance.
	/*!	@param[in] scene PhysX scene the new @ref WorldService will be responsible for controlling.
		@param[in] controllerManager PhysX controller manager the new @ref WorldService will use to create character controller instances.
		@param[in] world @ref World that controls the lifetime of the @ref WorldService.*/
		WorldService( PhysX::UniquePointer<physx::PxScene> scene, PhysX::UniquePointer<physx::PxControllerManager> controllerManager, const Core::World& world );
	//!	Disable copying.
		WorldService( const WorldService& ) = delete;

		~WorldService();

	// - DEBUG/LOGGING INFORMATION -----------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	public:
		Eldritch2::Allocator&	GetArmatureAnimationLayerRootAllocator() const;

		Eldritch2::Allocator&	GetArmatureAllocator() const;

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStartVisitor ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const GameStopVisitor ) override;
		void	AcceptVisitor( Scripting::MessageBus& messageBus ) override;
		void	AcceptVisitor( Core::ServiceBlackboard& blackboard ) override;

	// ---------------------------------------------------

	public:
		void	OnPostScriptTick( Scheduling::JobFiber& executor ) override;

	// - PXSIMULATIONEVENTCALLBACK METHODS ---------------

	public:
		void	onConstraintBreak( physx::PxConstraintInfo* constraints, physx::PxU32 count ) override;

		void	onWake( physx::PxActor** actors, physx::PxU32 count ) override;

		void	onSleep( physx::PxActor** actors, physx::PxU32 count ) override;

		void	onContact( const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs ) override;

		void	onTrigger( physx::PxTriggerPair* pairs, physx::PxU32 count ) override;

	// - PXCONTACTMODIFYCALLBACK METHODS -----------------

	public:
		void	onContactModify( physx::PxContactModifyPair* const pairs, physx::PxU32 count ) override;

	// ---------------------------------------------------

	public:
		static physx::PxFilterFlags	FilterShader( physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
												  physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
												  physx::PxPairFlags& pairFlags,
												  const void* constantBlock, physx::PxU32 constantBlockSize );

	// ---------------------------------------------------

	public:
		static void	RegisterScriptApi( Scripting::ApiRegistrar& typeRegistrar );

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator					_animationLayerAllocator;
		mutable Eldritch2::ObjectPoolAllocator				_armatureAllocator;

		PhysX::UniquePointer<physx::PxScene>				_scene;
		PhysX::UniquePointer<physx::PxControllerManager>	_controllerManager;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/WorldService.inl>
//------------------------------------------------------------------//