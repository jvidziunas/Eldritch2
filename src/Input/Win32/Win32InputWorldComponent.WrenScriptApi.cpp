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
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2