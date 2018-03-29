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
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Input {
		namespace Win32 {
			ET_DECLARE_WREN_CLASS( DeviceCoordinator );
		}
	}
}

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	using namespace ::Eldritch2::Scripting::Wren;

	void Win32InputWorldComponent::AcceptVisitor( ApiBuilder& api ) {
		ET_REGISTER_WREN_CLASS( DeviceCoordinator, api );

		api.CreateVariable<DeviceCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "MouseCoordinator" );
		api.CreateVariable<DeviceCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "KeyboardCoordinator" );
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2