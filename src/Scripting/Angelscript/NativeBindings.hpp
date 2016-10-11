/*==================================================================*\
  NativeBindings.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

class	asIScriptEngine;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	void	RegisterCMath( asIScriptEngine* const engine );

	void	RegisterAlgorithms( asIScriptEngine* const engine );

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2