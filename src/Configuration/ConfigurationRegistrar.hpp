/*==================================================================*\
  ConfigurationRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurationDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Configuration {

	class ConfigurationRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ConfigurationRegistrar instance.
	/*! @param[in] database The @ref ConfigurationDatabase the @ref ConfigurationRegistrar should register @ref ConfigurableVariable instances into. */
		ConfigurationRegistrar( Configuration::ConfigurationDatabase& database );
	//!	Constructs this @ref ConfigurationRegistrar instance.
		ConfigurationRegistrar( const ConfigurationRegistrar& ) = default;

		~ConfigurationRegistrar() = default;

	// ---------------------------------------------------

	public:
		ConfigurationRegistrar&	BeginSection( Eldritch2::Utf8Literal sectionName );

	// ---------------------------------------------------

	public:
		ConfigurationRegistrar&	Register( Eldritch2::Utf8Literal name, const Configuration::ConfigurationDatabase::PropertySetter& setter );
		template <typename PodValue>
		ConfigurationRegistrar&	Register( Eldritch2::Utf8Literal name, PodValue& value );

		ConfigurationRegistrar&	RegisterDynamicKeySetter( const Configuration::ConfigurationDatabase::DynamicKeySetter& setter );

	// ---------------------------------------------------

	//!	Disable assignment.
		ConfigurationRegistrar&	operator=( const ConfigurationRegistrar& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Configuration::ConfigurationDatabase&	_database;
		Eldritch2::Utf8Literal					_section;
	};

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/ConfigurationRegistrar.inl>
//------------------------------------------------------------------//