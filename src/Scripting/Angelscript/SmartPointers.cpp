/*==================================================================*\
  SmartPointers.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/SmartPointers.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace Detail {

	void ScriptObjectDeleter::operator()( ::asIScriptObject* const object ) {
		object->GetEngine()->ReleaseScriptObject( object, object->GetObjectType() );
	}

// ---------------------------------------------------

	void ScriptEngineDeleter::operator()( ::asIScriptEngine* const engine ) {
		engine->ShutDownAndRelease();
	}

// ---------------------------------------------------

	void ScriptModuleDeleter::operator()( ::asIScriptModule* const module ) {
		module->Discard();
	}
	
}	// namespace Detail
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2



