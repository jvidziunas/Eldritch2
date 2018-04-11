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
#include <Input/InputBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Input {
		ET_DECLARE_WREN_CLASS( InputBus );
	}
}

namespace Eldritch2 {
namespace Input {
namespace XInput {

	using namespace ::Eldritch2::Scripting::Wren;

	void XInputWorldComponent::AcceptVisitor( ApiBuilder& api ) {
		ET_REGISTER_WREN_CLASS( InputBus, api );

		api.CreateVariable<InputBus>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "GamepadBus" );
	}

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2