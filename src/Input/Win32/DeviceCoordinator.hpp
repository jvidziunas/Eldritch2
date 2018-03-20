/*==================================================================*\
  DeviceCoordinator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/InputDevices.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	class DeviceCoordinator {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		DeviceCoordinator( const DeviceCoordinator& ) = delete;
	//!	Constructs this @ref DeviceCoordinator instance.
		DeviceCoordinator();
	};

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2