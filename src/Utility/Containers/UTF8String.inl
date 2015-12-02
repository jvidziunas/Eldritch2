/*==================================================================*\
  UTF8String.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <utfcpp/v2_0/source/utf8/unchecked.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	
	template <class Allocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( AllocatorType&& allocator ) : String<::Eldritch2::UTF8Char, Allocator>( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator ) : String<::Eldritch2::UTF8Char, Allocator>( string, stringEnd, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	template <size_t literalLength>
	ETInlineHint UTF8String<Allocator>::UTF8String( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator ) : String<::Eldritch2::UTF8Char, Allocator>( stringLiteral, ::std::move( allocator ) ) {}
	
// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( const ::Eldritch2::UTF8String<AlternateAllocator>& string, AllocatorType&& allocator ) : String<::Eldritch2::UTF8Char, Allocator>( string, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( ::Eldritch2::UTF8String<Allocator>&& sourceString ) : String<::Eldritch2::UTF8Char, Allocator>( ::std::move( sourceString ) ) {}

}	// namespace Eldritch2