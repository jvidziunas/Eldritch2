/*==================================================================*\
  DeviceCoordinator.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/DeviceCoordinator.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	ET_IMPLEMENT_WREN_CLASS( DeviceCoordinator ) {
		api.CreateClass<DeviceCoordinator>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "DeviceCoordinator",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2