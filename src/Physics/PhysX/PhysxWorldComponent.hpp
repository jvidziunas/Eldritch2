/*==================================================================*\
  PhysxWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/CpuDispatcher.hpp>
#include <Core/WorldComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {
			class	PhysicsScene;
		}
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class PhysxWorldComponent : public Core::WorldComponent, public CpuDispatcher {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		PhysxWorldComponent( const PhysxWorldComponent& ) = delete;
	//!	Constructs this @ref PhysXWorldComponent instance.
	/*!	@param[in] owner @ref World that controls the lifetime of the @ref PhysxWorldComponent.*/
		PhysxWorldComponent( const Core::World& owner );

		~PhysxWorldComponent() = default;

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const EarlyTickVisitor& ) override;
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const LateTickVisitor& ) override;
		void	AcceptVisitor( Scripting::Wren::ApiBuilder& api ) override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PhysxWorldComponent&	operator=( const PhysxWorldComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Mutable so logs can be written even in const methods.
		mutable Logging::ChildLog	_log;
		bool						_shouldJoinScene;

		PhysicsScene*				_scene;
	};

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2