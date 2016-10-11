/*==================================================================*\
  VirtualMachineContext.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/VirtualMachineContext.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	VirtualMachineContext::VirtualMachineContext( asIScriptGeneric& scriptInterface ) : _scriptInterface( scriptInterface ) {}

// ---------------------------------------------------

	void* VirtualMachineContext::GetArgumentPointer( size_t argumentIndex ) const {
		return _scriptInterface.GetAddressOfArg( argumentIndex );
	}

// ---------------------------------------------------

	void* VirtualMachineContext::GetResultAddress() const {
		return _scriptInterface.GetAddressOfReturnLocation();
	}

// ---------------------------------------------------

	void VirtualMachineContext::ThrowScriptException( const Utf8Char* const messsage ) {
		asGetActiveContext()->SetException( messsage );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2