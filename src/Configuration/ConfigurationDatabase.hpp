/*==================================================================*\
  ConfigurationDatabase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/HashMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationPublishingInitializationVisitor;
		class	ConfigurableVariable;
	}
}

namespace Eldritch2 {
namespace Configuration {

	class ConfigurationDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct VariableKey : public ::Eldritch2::Pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref VariableKey instance.
			ETInlineHint VariableKey( const ::Eldritch2::UTF8Char* const section = "", const ::Eldritch2::UTF8Char* const name = "" );

			~VariableKey() = default;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref ConfigurationDatabase instance.
		/*! @param[in] allocator The @ref Allocator instance the @ref ConfigurationDatabase should use to perform sub-allocations.
			*/
		ConfigurationDatabase( ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ConfigurationDatabase instance.
		~ConfigurationDatabase() = default;

	// ---------------------------------------------------

		void	SetValue( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& section, const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& name, const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& value );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::HashMap<VariableKey, Configuration::ConfigurableVariable*>	_variableDirectory;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ConfigurationPublishingInitializationVisitor;
	};

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t	GetHashCode( const ConfigurationDatabase::VariableKey& key, const size_t seed );

	ETInlineHint ETNoAliasHint bool		operator==( const ConfigurationDatabase::VariableKey& left, const ConfigurationDatabase::VariableKey& right );

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/ConfigurationDatabase.inl>
//------------------------------------------------------------------//