/*==================================================================*\
  XInputEngineComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/XInputEngineComponent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/InputApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	ET_DECLARE_WREN_CLASS(DeviceLocator);
}} // namespace Eldritch2::Input

namespace Eldritch2 { namespace Input { namespace XInput {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	void XInputEngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		static ETConstexpr StringSpan InputModule(ET_BUILTIN_WREN_MODULE_NAME(Input));

		ET_REGISTER_WREN_CLASS(DeviceLocator, api, InputModule);

		_gamepads = api.DefineVariable<DeviceLocator>("Gamepads", InputModule);
	}

}}} // namespace Eldritch2::Input::XInput
