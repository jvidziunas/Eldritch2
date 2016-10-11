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

	void ScriptObjectDeleter::operator()( asIScriptObject* const object ) {
		if( nullptr == object ) {
			return;
		}

		object->GetEngine()->ReleaseScriptObject( object, object->GetObjectType() );
	}

// ---------------------------------------------------

	void ScriptObjectDeleter::operator()( asIScriptEngine* const engine ) {
		if( nullptr == engine ) {
			return;
		}

		engine->ShutDownAndRelease();
	}

// ---------------------------------------------------

	void ScriptObjectDeleter::operator()( asIScriptModule* const module ) {
		if( nullptr == module ) {
			return;
		}

		module->Discard();
	}
	
}	// namespace Detail
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2



