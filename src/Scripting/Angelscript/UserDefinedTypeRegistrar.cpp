/*==================================================================*\
  UserDefinedTypeRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/UserDefinedTypeRegistrar.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	UserDefinedTypeRegistrar::UserDefinedTypeRegistrar( ::asIScriptEngine& scriptEngine ) : _scriptEngine( scriptEngine ) {}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2
