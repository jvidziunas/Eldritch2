/*==================================================================*\
  SmartPointers.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <memory>
//------------------------------------------------------------------//

class	asIScriptObject;
class	asIScriptEngine;
class	asIScriptModule;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace Detail {

	struct ScriptObjectDeleter {
		ScriptObjectDeleter() = default;
		~ScriptObjectDeleter() = default;

		void	operator()( ::asIScriptObject* const object );
	};

// ---------------------------------------------------

	struct ScriptEngineDeleter {
		ScriptEngineDeleter() = default;
		~ScriptEngineDeleter() = default;

		void	operator()( ::asIScriptEngine* const engine );
	};

// ---------------------------------------------------

	struct ScriptModuleDeleter {
		ScriptModuleDeleter() = default;
		~ScriptModuleDeleter() = default;

		void	operator()( ::asIScriptModule* const module );
	};
	
}	// namespace Detail

	using ObjectHandle	= ::std::unique_ptr<::asIScriptObject, Detail::ScriptObjectDeleter>;
	using EngineHandle	= ::std::unique_ptr<::asIScriptEngine, Detail::ScriptEngineDeleter>;
	using ModuleHandle	= ::std::unique_ptr<::asIScriptModule, Detail::ScriptModuleDeleter>;

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2