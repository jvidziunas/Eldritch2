/*==================================================================*\
  ConfigurationDatabase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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

	namespace Configuration	= ::Eldritch2::Configuration;

// ---------------------------------------------------

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
		struct Key {
		// - TYPE PUBLISHING ---------------------------------

			struct Hash : public Utility::StringHash {
				ETInlineHint ETNoAliasHint size_t	operator()( const Key& key ) const;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref Key instance.
			ETInlineHint Key( const ::Eldritch2::UTF8Char* const section = "", const ::Eldritch2::UTF8Char* const name = "" );

		// ---------------------------------------------------

			ETInlineHint ETNoAliasHint bool	operator==( const Key& right ) const;

		// - DATA MEMBERS ------------------------------------

			const ::Eldritch2::UTF8Char*	section;
			const ::Eldritch2::UTF8Char*	name;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::UnorderedMap<Key, Configuration::ConfigurableVariable*, Key::Hash>	_variableDirectory;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ::Eldritch2::Configuration::ConfigurationPublishingInitializationVisitor;
	};

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/ConfigurationDatabase.inl>
//------------------------------------------------------------------//