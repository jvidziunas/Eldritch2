/*==================================================================*\
  XInputWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/XInputWorldComponent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/DeviceLocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	ET_DECLARE_WREN_CLASS(DeviceLocator);
}} // namespace Eldritch2::Input

namespace Eldritch2 { namespace Input { namespace XInput {

	using namespace ::Eldritch2::Scripting::Wren;

	void XInputWorldComponent::DefineScriptApi(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(DeviceLocator, api);
		//	api.CreateVariable<DeviceLocator>(ET_BUILTIN_WREN_MODULE_NAME(Input), "Gamepads");
	}

}}} // namespace Eldritch2::Input::XInput
