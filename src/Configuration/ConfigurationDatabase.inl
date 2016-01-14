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

	ETInlineHint ConfigurationDatabase::VariableKey::VariableKey( const ::Eldritch2::UTF8Char* const section, const ::Eldritch2::UTF8Char* const name ) : Pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*>( section, name ) {}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t GetHashCode( const ConfigurationDatabase::VariableKey& key, const size_t seed ) {
		return ::Eldritch2::HashMemory( key.second, ::Eldritch2::StringLength( key.second ), ::Eldritch2::HashMemory( key.first, ::Eldritch2::StringLength( key.first ), seed ) );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint bool operator==( const ConfigurationDatabase::VariableKey& left, const ConfigurationDatabase::VariableKey& right ) {
		return ::Eldritch2::EqualityCompareString( left.first, right.first ) && ::Eldritch2::EqualityCompareString( left.second, right.second );
	}

}	// namespace Configuration
}	// namespace Eldritch2