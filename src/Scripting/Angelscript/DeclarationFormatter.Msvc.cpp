/*==================================================================*\
  DeclarationFormatter.Msvc.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/TypeHandle.hpp>
//------------------------------------------------------------------//

#if( ET_COMPILER_IS_MSVC )
namespace Eldritch2 {
namespace Scripting {

	ETPureFunctionHint Eldritch2::Utf8Literal NameFromTypeHandle( TypeHandle typeHandle ) {
		const auto	strippedName( FindLastInstance( typeHandle.GetName(), "::" ) );

		return Utf8Literal( strippedName ? strippedName + 2 : typeHandle.GetName() );
	}

}	// namespace Scripting
}	// namespace Eldritch2
#endif