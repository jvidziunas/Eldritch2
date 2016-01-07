/*==================================================================*\
  SystemString.inl
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
	
	template <class Allocator>
	ETInlineHint SystemString<Allocator>::SystemString( AllocatorType&& allocator ) : String<SystemChar, Allocator>( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint SystemString<Allocator>::SystemString( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator ) : String<SystemChar, Allocator>( string, stringEnd, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	template <size_t literalLength>
	ETInlineHint SystemString<Allocator>::SystemString( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator ) : String<SystemChar, Allocator>( stringLiteral, ::std::move( allocator ) ) {}
	
// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint SystemString<Allocator>::SystemString( const ::Eldritch2::SystemString<AlternateAllocator>& string, AllocatorType&& allocator ) : String<SystemChar, Allocator>( string, ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint SystemString<Allocator>::SystemString( ::Eldritch2::SystemString<Allocator>&& sourceString ) : String<SystemChar, Allocator>( ::std::move( sourceString ) ) {}

}	// namespace Eldritch2

