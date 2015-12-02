/*==================================================================*\
  StringHash.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/String.hpp>
#include <Utility/Hash.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Character = ::Eldritch2::UTF8Char>
	class StringHash : public ::Eldritch2::Hash<Character*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StringHash instance.
		StringHash() = default;

		~StringHash() = default;

	// ---------------------------------------------------

		// Pull additional overloads into scope for interoperability with C strings.
		using ::Eldritch2::Hash<Character*>::operator();

		template <typename Allocator>
		ETNoAliasHint size_t	operator()( const String<Character, Allocator>& string, const size_t seed = static_cast<size_t>(0) ) const;
		
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/StringHash.inl>
//------------------------------------------------------------------//