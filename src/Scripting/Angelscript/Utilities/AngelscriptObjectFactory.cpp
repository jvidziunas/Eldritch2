/*==================================================================*\
  AngelscriptObjectFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/Utilities/AngelscriptObjectFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	AngelscriptObjectFactory::AngelscriptObjectFactory( ::asIObjectType* const objectType, ::asIScriptContext* const scriptContext ) : _objectType( objectType ), _scriptContext( scriptContext ) {}

// ---------------------------------------------------

	AngelscriptObjectFactory::~AngelscriptObjectFactory() {}

}	// namespace Scripting
}	// namespace Eldritch2
