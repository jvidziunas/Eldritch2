/*==================================================================*\
  AngelscriptUserDefinedTypeRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptUserDefinedTypeRegistrar.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {

	AngelscriptUserDefinedTypeRegistrar::AngelscriptUserDefinedTypeRegistrar( ::asIScriptEngine& scriptEngine ) : _scriptEngine( scriptEngine ) {}

// ---------------------------------------------------

	AngelscriptUserDefinedTypeRegistrar::~AngelscriptUserDefinedTypeRegistrar() {}

}	// namespace Scripting
}	// namespace Eldritch2
