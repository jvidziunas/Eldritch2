/*==================================================================*\
  ConfigurationPublishingInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationDatabase;
		class	ConfigurableVariable;
	}
}

namespace Eldritch2 {
namespace Configuration {

	namespace Configuration	= ::Eldritch2::Configuration;

// ---------------------------------------------------

	class ConfigurationPublishingInitializationVisitor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this @ref ConfigurationPublishingInitializationVisitor instance.
		/*! @param[in] database The @ref ConfigurationDatabase the @ref ConfigurationPublishingInitializationVisitor should register @ref ConfigurableVariable instances into.
			*/
		ConfigurationPublishingInitializationVisitor( Configuration::ConfigurationDatabase& database );

		// Destroys this @ref ConfigurationPublishingInitializationVisitor instance.
		~ConfigurationPublishingInitializationVisitor() = default;

	// ---------------------------------------------------

		Configuration::ConfigurationPublishingInitializationVisitor&	PushSection( const ::Eldritch2::UTF8Char* const sectionName );

		Configuration::ConfigurationPublishingInitializationVisitor&	Register( const::Eldritch2::UTF8Char* const name, Configuration::ConfigurableVariable& variable );

	// - DATA MEMBERS ------------------------------------

	private:
		Configuration::ConfigurationDatabase&	_database;
		const ::Eldritch2::UTF8Char*			_section;
	};

}	// namespace Configuration
}	// namespace Eldritch2