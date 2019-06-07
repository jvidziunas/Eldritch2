/*==================================================================*\
  ActionSetClient.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/InputApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {
	class ScriptExecutor;
}}} // namespace Eldritch2::Scripting::Wren

struct WrenHandle;

namespace Eldritch2 { namespace Scripting { namespace Wren {

	struct InputAction {
		WrenHandle* closure;
		union {
			Atomic<uint64> combinedWeight;
			struct {
				Atomic<uint32> pressed, released;
			} weights;
		};
	};

	// ---

	class ActionSetClient : public ArrayList<InputAction>, public Input::InputClient {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ActionSetClient(const ActionSetClient&) = delete;
		//!	Constructs this @ref ActionSetClient instance.
		ActionSetClient() ETNoexceptHint;

		~ActionSetClient();

		// ---------------------------------------------------

	public:
		//!	Invokes input event handlers for all actions attached to this @ref ActionSetClient.
		/*!	@param[in] context Wren VM context that will execute script code.
			@returns true if all listeners executed successfully, or false if a runtime exception occurred. */
		bool DispatchEvents(ScriptExecutor& context);

		// ---------------------------------------------------

	public:
		Result BindResources(ScriptExecutor& context, int bindingsSlot);

		void FreeResources(ScriptExecutor& context) ETNoexceptHint;

		// ---------------------------------------------------

	protected:
		void Activate(Input::Action id, int32 amount) override;

		void OnConnect(const Input::InputDevice& device) override;

		void OnDisconnect(const Input::InputDevice& device) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ActionSetClient& operator=(const ActionSetClient&) = delete;

		// - DATA MEMBERS ------------------------------------

	public:
		ArrayList<Pair<Input::DeviceLocator*, Input::DeviceId>> _links;

		// ---------------------------------------------------

		friend void Swap(ActionSetClient&, ActionSetClient&) = delete;
	};

}}} // namespace Eldritch2::Scripting::Wren
