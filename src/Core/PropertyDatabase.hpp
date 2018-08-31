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
		using DynamicSetter = Function<void(StringView /*name*/, StringView /*value*/) ETNoexceptHint>;
		using Setter        = Function<void(StringView /*value*/) ETNoexceptHint>;
		using SetterKey     = Pair<StringView, StringView>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PropertyDatabase(const PropertyDatabase&) = delete;
		//! Constructs this @ref PropertyDatabase instance.
		PropertyDatabase();

		~PropertyDatabase() = default;

		// ---------------------------------------------------

	public:
		bool SetValue(StringView group, StringView name, StringView value) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PropertyDatabase& operator=(const PropertyDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		CachingHashMap<SetterKey, Setter>         _settersByKey;
		CachingHashMap<StringView, DynamicSetter> _dynamicSettersByGroup;

		// - FRIEND CLASS DECLARATION ------------------------

		friend class Core::PropertyRegistrar;
	};

}} // namespace Eldritch2::Core
