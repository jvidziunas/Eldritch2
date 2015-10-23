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

	ETInlineHint ConfigurationDatabase::Key::Key( const ::Eldritch2::UTF8Char* const section, const ::Eldritch2::UTF8Char* const name ) : section( section ), name( name ) {}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t ConfigurationDatabase::Key::Hash::operator()( const Configuration::ConfigurationDatabase::Key& variableKey ) const {
		return static_cast<const Utility::StringHash&>(*this)( variableKey.section, static_cast<const Utility::StringHash&>(*this)( variableKey.name ) );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint bool	ConfigurationDatabase::Key::operator==( const Key& right ) const {
		Utility::StringEqualComparator<::Eldritch2::UTF8Char>	comparator;

		return comparator( name, right.name ) && comparator( section, right.section );
	}

}	// namespace Configuration
}	// namespace Eldritch2