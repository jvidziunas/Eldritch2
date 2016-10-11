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
#include <Utility/StringLiteral.hpp>
//------------------------------------------------------------------//
#include <functional>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Configuration {
		class	ConfigurationRegistrar;
	}

	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Configuration {

	class ConfigurationDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using UnknownKeyHandler	= std::function<void ( Eldritch2::Utf8Literal /*sectionName*/, Eldritch2::Utf8Literal /*valueName*/, Eldritch2::Range<const Eldritch2::Utf8Char*> /*value*/ )>;
		using DynamicKeySetter	= std::function<void ( Eldritch2::Utf8Literal /*valueName*/, Eldritch2::Range<const Eldritch2::Utf8Char*> /*value*/ )>;
		using PropertySetter	= std::function<void ( Eldritch2::Range<const Eldritch2::Utf8Char*> /*value*/ )>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref ConfigurationDatabase instance.
	/*! @param[in] unknownKeyHandler Handler function the ConfigurationDatabase should use to report unknown keys.
		@param[in] allocator The @ref Allocator instance the @ref ConfigurationDatabase should use to perform sub-allocations. */
		ConfigurationDatabase( const UnknownKeyHandler& unknownKeyHandler, Eldritch2::Allocator& allocator );
	//! Constructs this @ref ConfigurationDatabase instance.
	/*! @param[in] allocator The @ref Allocator instance the @ref ConfigurationDatabase should use to perform sub-allocations.
		@remarks The default key handler will be set to a no-op function. */
		ConfigurationDatabase( Eldritch2::Allocator& allocator );
	//!	Disable copying.
		ConfigurationDatabase( const ConfigurationDatabase& ) = delete;

		~ConfigurationDatabase() = default;

	// ---------------------------------------------------

	public:
		void	SetProperty( Eldritch2::Range<const Eldritch2::Utf8Char*> section, Eldritch2::Range<const Eldritch2::Utf8Char*> name, Eldritch2::Range<const Eldritch2::Utf8Char*> value );

	// ---------------------------------------------------

	public:
		void	SetUnknownKeyHandler( const UnknownKeyHandler& handler );

	// ---------------------------------------------------

	//!	Disable assignment.
		ConfigurationDatabase&	operator=( const ConfigurationDatabase& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::HashMap<Eldritch2::Pair<Eldritch2::Utf8Literal, Eldritch2::Utf8Literal>, PropertySetter>	_propertySetters;
		Eldritch2::HashMap<Eldritch2::Utf8Literal, DynamicKeySetter>										_dynamicKeySetters;

		UnknownKeyHandler																					_unknownKeyHandler;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class Eldritch2::Configuration::ConfigurationRegistrar;
	};

}	// namespace Configuration
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Configuration/ConfigurationDatabase.inl>
//------------------------------------------------------------------//