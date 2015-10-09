/*==================================================================*\
  StringOperators.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/MemStdLib.hpp>
//------------------------------------------------------------------//

namespace Eldritch2Detail {
	::Eldritch2::uint32	FastHash( const void* bufferToHash, const size_t lengthInBytes, const ::Eldritch2::uint32 seed );
	::Eldritch2::uint64	FastHash( const void* bufferToHash, const size_t lengthInBytes, const ::Eldritch2::uint64 seed );
}

namespace Eldritch2 {
namespace Utility {

	template <typename Character>
	ETNoAliasHint bool StringComparator<Character>::operator()( const Character* left, const Character* right ) const {
		return ::Eldritch2::CompareString( left, right ) < 0;
	}

// ---------------------------------------------------

	template <typename Character>
	ETNoAliasHint bool StringEqualComparator<Character>::operator()( const Character* left, const Character* right ) const {
		return ::Eldritch2::EqualityCompareString( left, right );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t StringHash::operator()( const ::Eldritch2::UTF8Char* string, const size_t seed ) const {
		return ::Eldritch2Detail::FastHash( string, ::Eldritch2::StringLength( string ), seed );
	}

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t StringHash::operator()( const ::Eldritch2::UTF8Char* string, const ::Eldritch2::UTF8Char* stringEnd, const size_t seed ) const {
		return ::Eldritch2Detail::FastHash( string, static_cast<size_t>(stringEnd - string), seed );
	}

}	// namespace Utility
}	// namespace Eldritch2