/*==================================================================*\
  ActionSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/InputDevice.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	class DeviceLocator;
}} // namespace Eldritch2::Input

struct WrenHandle;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	class ActionSet : public Input::InputHandler {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : int32 { InitialWeight = 0 };

		enum ActionFlags : uint32 {
			None     = 0u,
			Pressed  = 1u,
			Released = 1u << 1u
		};

		// ---

	public:
		struct Action {
			WrenHandle* closure;
			int32       weight;
			uint32      flags;
		};

		// ---

		struct DeviceBinding {
			Input::DeviceLocator* locator;
			Input::DeviceId       id;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ActionSet(const ActionSet&) = delete;
		//!	Constructs this @ref ActionSet instance.
		ActionSet();

		~ActionSet();

		// ---------------------------------------------------

	public:
		void Activate(Input::ActionId id, int32 amount) override;

		void OnConnect() override;

		void OnDisconnect() override;

		// ---------------------------------------------------

	public:
		//!	Invokes input event handlers for all actions attached to this @ref ActionSet.
		/*!	@param[in] vm Wren virtual machine object that will execute script code.
			@param[in] arity3Call Wren call handle with signature `call(_,_,_)`
			@returns true if all listeners executed successfully, or false if a runtime exception occurred. */
		bool DispatchEvents(WrenVM* vm, WrenHandle* arity3Call);

		// ---------------------------------------------------

	public:
		bool TryAcquireDevice(Input::DeviceLocator& locator, Input::DeviceId id);

		void ReleaseDevices();

		// ---------------------------------------------------

	public:
		void BindResources(WrenVM* vm, int bindingsSlot);

		void FreeResources(WrenVM* vm);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ActionSet& operator=(const ActionSet&) = delete;

		// - DATA MEMBERS ------------------------------------

	public:
		ArrayList<Action>        _actions;
		ArrayList<DeviceBinding> _bindings;
	};

}}} // namespace Eldritch2::Scripting::Wren
