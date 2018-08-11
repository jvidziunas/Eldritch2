/*==================================================================*\
  XInputEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
//------------------------------------------------------------------//
#include <XInput.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input { namespace XInput {

	class XInputEngineComponent : public Core::EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		XInputEngineComponent(const XInputEngineComponent&) = delete;
		//!	Constructs this @ref XInputEngineComponent instance.
		XInputEngineComponent(const ObjectLocator& services);

		~XInputEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	protected:
		UniquePointer<Core::WorldComponent> CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) override;

		void TickEarly(Scheduling::JobExecutor& executor) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		XInputEngineComponent& operator=(const XInputEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		XINPUT_STATE _gamepads[4u];
	};

}}} // namespace Eldritch2::Input::XInput
