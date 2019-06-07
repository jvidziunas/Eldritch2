/*==================================================================*\
  PropertyDatabase.hpp
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

namespace Eldritch2 { namespace Core {

	class PropertyDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using DynamicSetter = Function<void(StringSpan /*name*/, StringSpan /*value*/)>;
		using Setter        = Function<void(StringSpan /*value*/)>;
		using SetterKey     = Pair<StringSpan, StringSpan>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PropertyDatabase(const PropertyDatabase&) = default;
		//! Constructs this @ref PropertyDatabase instance.
		PropertyDatabase(PropertyDatabase&&) ETNoexceptHint = default;
		//! Constructs this @ref PropertyDatabase instance.
		PropertyDatabase() ETNoexceptHint;

		~PropertyDatabase() = default;

		// ---------------------------------------------------

	public:
		bool SetValue(StringSpan group, StringSpan name, StringSpan value) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PropertyDatabase& operator=(const PropertyDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		CachingHashMap<SetterKey, Setter>         _settersByKey;
		CachingHashMap<StringSpan, DynamicSetter> _dynamicSettersByGroup;

		// - FRIEND CLASS DECLARATION ------------------------

		friend class PropertyApiBuilder;
	};

}} // namespace Eldritch2::Core
