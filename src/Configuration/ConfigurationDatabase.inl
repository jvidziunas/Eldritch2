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
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Configuration {

	ETInlineHint ConfigurationDatabase::Key::Key( const ::Eldritch2::UTF8Char* const section, const ::Eldritch2::UTF8Char* const name ) : Pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*>( section, name ) {}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint bool	ConfigurationDatabase::Key::operator==( const Key& right ) const {
		return ::Eldritch2::EqualityCompareString( first, right.first ) && ::Eldritch2::EqualityCompareString( second, right.second );
	}

}	// namespace Configuration
}	// namespace Eldritch2