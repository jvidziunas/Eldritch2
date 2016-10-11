/*==================================================================*\
  EngineService.Controller.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/XInput/EngineService.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if defined(_WIN32_WINNT_WIN8) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	ET_LINK_LIBRARY( "XInput.lib" )
#else 
	ET_LINK_LIBRARY( "XInput9_1_0.lib" )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;

namespace Eldritch2 {
namespace Input {
namespace XInput {

	EngineService::Controller::Controller( DWORD controllerIndex ) : _controllerIndex( controllerIndex ) {
		SampleInput();
	}

// ---------------------------------------------------

	void EngineService::Controller::SampleInput() {
		if( Controller::InvalidControllerIndex == _controllerIndex ) {
			return;
		}

		XInputGetState( _controllerIndex, &_state );
	}

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2