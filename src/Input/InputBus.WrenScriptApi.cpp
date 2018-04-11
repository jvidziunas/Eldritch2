/*==================================================================*\
  InputBus.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Input/InputBus.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {

	ET_IMPLEMENT_WREN_CLASS( InputBus ) {
		api.CreateClass<InputBus>( ET_BUILTIN_WREN_MODULE_NAME( Input ), "InputBus",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Input
}	// namespace Eldritch2