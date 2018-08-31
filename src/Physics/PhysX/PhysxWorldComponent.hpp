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

namespace Eldritch2 { namespace Physics { namespace PhysX {
	class PhysicsScene;
}}} // namespace Eldritch2::Physics::PhysX

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysxWorldComponent : public Core::WorldComponent, public CpuDispatcher {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysXWorldComponent instance.
		PhysxWorldComponent(const ObjectLocator& services);
		//!	Disable copy construction.
		PhysxWorldComponent(const PhysxWorldComponent&) = delete;

		~PhysxWorldComponent() = default;

		// ---------------------------------------------------

	protected:
		void OnFixedRateTickEarly(Scheduling::JobExecutor& executor, MicrosecondTime duration) override;

		void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime duration) override;

		// ---------------------------------------------------

	protected:
		void BindResources(Scheduling::JobExecutor& executor) override;

		void DefineScriptApi(Scripting::Wren::ApiBuilder& api) override;

		void FreeResources(Scheduling::JobExecutor& executor) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PhysxWorldComponent& operator=(const PhysxWorldComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::ChildLog _log;
		PhysicsScene*             _scene;
	};

}}} // namespace Eldritch2::Physics::PhysX
