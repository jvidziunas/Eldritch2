/*==================================================================*\
  Win32InputEngineComponent.WrenScriptApi.cpp
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
#include <Input/Win32/Win32InputEngineComponent.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/InputApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Input {
	ET_DECLARE_WREN_CLASS(DeviceLocator);
}} // namespace Eldritch2::Input

namespace Eldritch2 { namespace Input { namespace Win32 {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	void Win32InputEngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		static ETConstexpr StringSpan InputModule(ET_BUILTIN_WREN_MODULE_NAME(Input));

		ET_REGISTER_WREN_CLASS(DeviceLocator, api, InputModule);

		_keyboards = api.DefineVariable<DeviceLocator>("Keyboards", InputModule);
		_mice      = api.DefineVariable<DeviceLocator>("Mice", InputModule);
	}

}}} // namespace Eldritch2::Input::Win32
