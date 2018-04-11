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
namespace Win32 {

	using namespace ::Eldritch2::Scripting::Wren;

	void Win32InputWorldComponent::AcceptVisitor( ApiBuilder& api ) {
		ET_REGISTER_WREN_CLASS( InputBus, api );

		api.CreateVariable<InputBus>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "KeyboardBus" );
		api.CreateVariable<InputBus>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "MouseBus" );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2