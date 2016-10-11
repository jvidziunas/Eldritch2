/*==================================================================*\
  ConfigurationDatabase.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	ETInlineHint void ConfigurationDatabase::SetUnknownKeyHandler( const UnknownKeyHandler& handler ) {
		_unknownKeyHandler = handler;
	}

}	// namespace Configuration
}	// namespace Eldritch2