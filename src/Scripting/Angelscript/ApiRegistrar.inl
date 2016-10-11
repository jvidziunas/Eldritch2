/*==================================================================*\
  UserDefinedTypeRegistrar.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ETInlineHint const Eldritch2::Utf8Char* ApiRegistrar::GetTypeName( Eldritch2::TypeHandle type ) const {
		return _declarationFormatter.GetName( type );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2