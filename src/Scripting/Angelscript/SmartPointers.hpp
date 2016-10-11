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
#include <Utility/UniquePointer.hpp>
//------------------------------------------------------------------//

class	asIScriptObject;
class	asIScriptEngine;
class	asIScriptModule;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace Detail {

	class ScriptObjectDeleter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ScriptObjectDeleter instance.
		ScriptObjectDeleter() = default;

		~ScriptObjectDeleter() = default;

	// ---------------------------------------------------

	public:
		void	operator()( asIScriptObject* const object );
		void	operator()( asIScriptEngine* const engine );
		void	operator()( asIScriptModule* const module );
	};
	
}	// namespace Detail

	template <typename AngelscriptObject>
	using UniquePointer = Eldritch2::UniquePointer<AngelscriptObject, Detail::ScriptObjectDeleter>;

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2