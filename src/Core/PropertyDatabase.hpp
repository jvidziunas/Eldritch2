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
	class PropertyRegistrar;
}} // namespace Eldritch2::Core

namespace Eldritch2 { namespace Core {

	class PropertyDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using DynamicSetter = Function<void(StringView<Utf8Char> /*name*/, StringView<Utf8Char> /*value*/)>;
		using SetterKey     = Pair<StringView<Utf8Char>, StringView<Utf8Char>>;
		using Setter        = Function<void(StringView<Utf8Char> /*value*/)>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PropertyDatabase(const PropertyDatabase&) = delete;
		//! Constructs this @ref PropertyDatabase instance.
		PropertyDatabase();

		~PropertyDatabase() = default;

		// ---------------------------------------------------

	public:
		bool SetValue(StringView<Utf8Char> group, StringView<Utf8Char> name, StringView<Utf8Char> value);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PropertyDatabase& operator=(const PropertyDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		CachingHashMap<SetterKey, Setter>                   _settersByKey;
		CachingHashMap<StringView<Utf8Char>, DynamicSetter> _dynamicSettersByGroup;

		// - FRIEND CLASS DECLARATION ------------------------

		friend class Core::PropertyRegistrar;
	};

}} // namespace Eldritch2::Core
