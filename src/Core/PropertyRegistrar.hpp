/*==================================================================*\
  PropertyRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/PropertyDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	class PropertyRegistrar {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PropertyRegistrar instance.
	/*! @param[in] database The @ref PropertyDatabase the @ref PropertyRegistrar should insert callbacks into. */
		PropertyRegistrar( PropertyDatabase& database );
	//!	Constructs this @ref PropertyRegistrar instance.
		PropertyRegistrar( const PropertyRegistrar& ) = default;

		~PropertyRegistrar() = default;

	// ---------------------------------------------------

	public:
	//!	Sets the active variable section used for coarse grouping.
	/*!	@param[in] sectionName Name of the configuration block variables will be added to.
		@returns (*this) for method chaining. */
		PropertyRegistrar&	BeginSection( Utf8Literal sectionName );

	// ---------------------------------------------------

	public:
	//!	Registers a function that will be called when the configuration parser encounters a variable with the specified name.
	/*!	@param[in] name Name of the variable you are interested in receiving notifications for.
		@param[in] setter Handler function that will optionally parse the input string into the correct final representation, changing
			client variables as appropriate.
		@returns (*this) for method chaining. */
		PropertyRegistrar&	WhenPropertyChanged( Utf8Literal name, PropertyDatabase::Setter setter );
		template <typename PodValue>
		PropertyRegistrar&	WhenPropertyChanged( Utf8Literal name, PodValue& value );

		PropertyRegistrar&	WhenDynamicPropertyChanged( PropertyDatabase::DynamicSetter setter );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		PropertyRegistrar&	operator=( const PropertyRegistrar& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		PropertyDatabase*	_database;
		Utf8Literal			_section;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/PropertyRegistrar.inl>
//------------------------------------------------------------------//