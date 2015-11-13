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
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/StringOperators.hpp>
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
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ConfigurationDatabase instance.
		/*! @param[in] allocator The @ref Allocator instance the @ref ConfigurationDatabase should use to perform sub-allocations.
			*/
		ConfigurationDatabase( ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ConfigurationDatabase instance.
		~ConfigurationDatabase() = default;

	// ---------------------------------------------------

		void	SetValue( const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& section, const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& name, const ::Eldritch2::Range<const ::Eldritch2::UTF8Char*>& value );

	// - TYPE PUBLISHING ---------------------------------

	private:
		struct Key : public ::Eldritch2::Pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref Key instance.
			ETInlineHint Key( const ::Eldritch2::UTF8Char* const section = "", const ::Eldritch2::UTF8Char* const name = "" );

			~Key() = default;

		// ---------------------------------------------------

			ETInlineHint ETNoAliasHint bool	operator==( const Key& right ) const;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::UnorderedMap<Key, Configuration::ConfigurableVariable*>	_variableDirectory;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ConfigurationPublishingInitializationVisitor;
	};

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/ConfigurationDatabase.inl>
//------------------------------------------------------------------//