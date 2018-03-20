/*==================================================================*\
  PhysxEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxAllocatorMixin.hpp>
#include <Physics/PhysX/PhysxErrorMixin.hpp>
#include <Core/EngineComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class PhysxEngineComponent : public Core::EngineComponent {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PhysxEngineComponent instance.
		PhysxEngineComponent( const Blackboard& services, Logging::Log& log );
	//!	Disable copy construction.
		PhysxEngineComponent( const PhysxEngineComponent& ) = delete;

		~PhysxEngineComponent() = default;

	// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		Result<UniquePointer<Core::WorldComponent>>	CreateWorldComponent( Allocator& allocator, const Core::World& world ) override;

		void										AcceptVisitor( Scheduling::JobExecutor& executor, const InitializationVisitor ) override;
		void										AcceptVisitor( Assets::AssetApiBuilder& factories ) override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PhysxEngineComponent&	operator=( const PhysxEngineComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//	Mutable so logs can be written even in const methods.
		mutable PhysxErrorMixin<Logging::ChildLog>		_log;
	//!	Mutable as objects allocated from the allocator are not directly part of the object's state.
		mutable PhysxAllocatorMixin<MallocAllocator>	_allocator;
	};

}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics