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

namespace Eldritch2 {
namespace Core {
	class PropertyRegistrar;
}
} // namespace Eldritch2::Core

namespace Eldritch2 { namespace Core {

	class PropertyDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using DynamicSetter = Function<void(const Utf8Char* /*name*/, Range<const Utf8Char*> /*value*/)>;
		using SetterKey     = Pair<Utf8Literal, Utf8Literal>;
		using Setter        = Function<void(Range<const Utf8Char*> /*value*/)>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PropertyDatabase(const PropertyDatabase&) = delete;
		//! Constructs this @ref PropertyDatabase instance.
		PropertyDatabase();

		~PropertyDatabase() = default;

		// ---------------------------------------------------

	public:
		bool SetValue(const Utf8Char* group, const Utf8Char* name, Range<const Utf8Char*> value);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PropertyDatabase& operator=(const PropertyDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		CachingHashMap<SetterKey, Setter>          _settersByKey;
		CachingHashMap<Utf8Literal, DynamicSetter> _dynamicSettersByGroup;

		// - FRIEND CLASS DECLARATION ------------------------

		friend class Core::PropertyRegistrar;
	};

}} // namespace Eldritch2::Core
