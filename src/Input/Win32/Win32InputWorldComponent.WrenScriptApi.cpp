/*==================================================================*\
  Win32InputWorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/Win32InputWorldComponent.hpp>
#include <Input/Win32/DeviceCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/InputBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	ET_DECLARE_WREN_CLASS(InputBus);
}} // namespace Eldritch2::Input

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Scripting::Wren;

	void Win32InputWorldComponent::AcceptVisitor(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(InputBus, api);

		DeviceCoordinator& devices(FindService<DeviceCoordinator>());

		api.CreateVariable<InputBus>(ET_BUILTIN_WREN_MODULE_NAME(Input), "Keyboard", devices.GetMutex(), devices.GetKeyboards());
		api.CreateVariable<InputBus>(ET_BUILTIN_WREN_MODULE_NAME(Input), "Mouse", devices.GetMutex(), devices.GetMice());
	}

}}} // namespace Eldritch2::Input::Win32
