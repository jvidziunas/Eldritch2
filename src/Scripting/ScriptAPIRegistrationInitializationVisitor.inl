/*==================================================================*\
  ScriptAPIRegistrationInitializationVisitor.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	template <typename... Arguments>
	ETInlineHint ScriptAPIRegistrationInitializationVisitor::ScriptAPIRegistrationInitializationVisitor( Arguments&&... arguments ) : UnderlyingTypeRegistrar( ::std::forward<Arguments>( arguments )... ) {}

}	// namespace Scripting
}	// namespace Eldritch2
