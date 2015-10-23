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

	void	RegisterCMathLibrary( ::asIScriptEngine* const engine );

	void	RegisterAlgorithmLibrary( ::asIScriptEngine* const engine );

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2