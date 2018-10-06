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
	class Context;
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
		// - TYPE PUBLISHING ---------------------------------

	public:
		using DeviceLink = UniquePointer<Input::InputDevice, Input::DeviceDisposer>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ActionSetClient(const ActionSetClient&) = delete;
		//!	Constructs this @ref ActionSetClient instance.
		ActionSetClient();

		~ActionSetClient();

		// ---------------------------------------------------

	public:
		void Activate(Input::ActionId id, int32 amount) override;

		void OnConnect(const Input::InputDevice& device) override;

		void OnDisconnect(const Input::InputDevice& device) override;

		// ---------------------------------------------------

	public:
		//!	Invokes input event handlers for all actions attached to this @ref ActionSetClient.
		/*!	@param[in] context Wren VM context that will execute script code.
			@returns true if all listeners executed successfully, or false if a runtime exception occurred. */
		bool DispatchEvents(Context& context);

		// ---------------------------------------------------

	public:
		void AcquireDevice(DeviceLink link);

		void ReleaseDevices();

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(Context& context, int bindingsSlot);

		void FreeResources(Context& context);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ActionSetClient& operator=(const ActionSetClient&) = delete;

		// - DATA MEMBERS ------------------------------------

	public:
		ArrayList<DeviceLink> _links;

		// ---------------------------------------------------

		friend void Swap(ActionSetClient&, ActionSetClient&) = delete;
	};

}}} // namespace Eldritch2::Scripting::Wren
