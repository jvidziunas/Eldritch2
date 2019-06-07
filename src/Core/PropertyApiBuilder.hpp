/*==================================================================*\
  PropertyApiBuilder.hpp
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

namespace Eldritch2 { namespace Core {

	class PropertyApiBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PropertyApiBuilder instance.
		PropertyApiBuilder(const PropertyApiBuilder&) = default;
		//!	Constructs this @ref PropertyApiBuilder instance.
		PropertyApiBuilder(PropertyApiBuilder&&) ETNoexceptHint = default;
		//!	Constructs this @ref PropertyApiBuilder instance.
		PropertyApiBuilder() ETNoexceptHint;

		~PropertyApiBuilder() = default;

		// ---------------------------------------------------

	public:
		//!	Sets the active variable group.
		/*!	@param[in] name Name of the configuration group variables will be added to.
			@returns (*this) for method chaining. */
		PropertyApiBuilder& DefineGroup(StringSpan name) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		//!	Registers a function that will be called when the configuration parser encounters a variable with the specified name.
		/*!	@param[in] name Name of the variable you are interested in receiving notifications for.
			@param[in] setter Handler function that will optionally parse the input string into the correct final representation, changing
				client variables as appropriate.
			@returns (*this) for method chaining. */
		PropertyApiBuilder& DefineProperty(StringSpan name, PropertyDatabase::Setter setter);

		PropertyApiBuilder& DefineDynamicProperty(PropertyDatabase::DynamicSetter setter);

		// ---------------------------------------------------

	public:
		ETConstexpr PropertyDatabase& GetProperties() ETNoexceptHint;

		// ---------------------------------------------------

		template <typename T>
		static ETConstexpr ETPureFunctionHint PropertyDatabase::Setter MakeSetter(T& variable);

		//!	Disable copy assignment.
		PropertyApiBuilder& operator=(const PropertyApiBuilder&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		PropertyDatabase _properties;
		StringSpan       _group;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/PropertyApiBuilder.inl>
//------------------------------------------------------------------//
