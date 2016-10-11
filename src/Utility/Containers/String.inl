/*==================================================================*\
  String.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//
#include <eastl/algorithm.h>
//------------------------------------------------------------------//

#if defined( CompareString )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_CompareString 1
#		pragma push_macro( "CompareString" )
#		undef CompareString
#	else
#		error CompareString macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif

namespace Eldritch2 {

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const CharacterType* const string, const CharacterType* const stringEnd, const AllocatorType& allocator ) : _underlyingContainer( string, stringEnd, allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const CharacterType* const string, const AllocatorType& allocator ) : _underlyingContainer( string, allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const String<Character, Allocator>& sourceString, const AllocatorType& allocator ) : _underlyingContainer( sourceString.Begin(), sourceString.End(), allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( SizeType reservedLength, const AllocatorType& allocator ) : _underlyingContainer( UnderlyingContainer::CtorDoNotInitialize(), reservedLength, allocator ) {}

// ---------------------------------------------------
	
	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const AllocatorType& allocator ) : _underlyingContainer( allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const String<Character, Allocator>& string ) : String( string, string.GetAllocator() ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( String<Character, Allocator>&& sourceString ) : _underlyingContainer( eastl::move( sourceString._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( ConstIterator startPosition, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		const auto	needleEnd( Eldritch2::FindEndOfString( needle ) );
		const auto	position( eastl::search( startPosition, _underlyingContainer.cend(), needle, needleEnd ) );

		return eastl::min( _underlyingContainer.cend(), position + ( needleEnd - needle ) );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( ConstIterator startPosition, const CharacterType* const needle ) const {
		return eastl::search( startPosition, _underlyingContainer.cend(), needle, Eldritch2::FindEndOfString( needle ) );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics semantics ) const {
		return FindFirstInstance( _underlyingContainer.cbegin(), needle, semantics );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( const CharacterType* const needle ) const {
		return FindFirstInstance( _underlyingContainer.cbegin(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( ConstIterator startPosition, CharacterType character ) const {
		return eastl::find( startPosition, _underlyingContainer.cend(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( CharacterType character ) const {
		return FindFirstInstance( _underlyingContainer.cbegin(), character );
	}

// ---------------------------------------------------
	

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics semantics ) const {
		return FindLastInstance( Begin(), needle, semantics );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( const CharacterType* const needle ) const {
		return FindLastInstance( Begin(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( ConstIterator startPosition, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		const auto	position( Eldritch2::FindLastInstance( startPosition, needle ) );

		return position ? position + Eldritch2::StringLength( needle ) : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( ConstIterator startPosition, const CharacterType* const needle ) const {
		const auto	position( Eldritch2::FindLastInstance( startPosition, needle ) );

		return position ? position : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( ConstIterator startPosition, CharacterType character ) const {
		const auto	position( Eldritch2::FindLastInstance( startPosition, character ) );

		return position ? position : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( CharacterType character ) const {
		return FindLastInstance( Begin(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Contains( const CharacterType* const needle ) const {
		return _underlyingContainer.cend() != FindFirstInstance( needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Contains( CharacterType character ) const {
		return _underlyingContainer.cend() != FindFirstInstance( character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::MakeLowerCase() {
		_underlyingContainer.make_lower();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::MakeUpperCase() {
		_underlyingContainer.make_upper();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::BeginsWith( const CharacterType* const needle ) const {
		const auto needleLength( Eldritch2::StringLength( needle ) );

		return (needleLength <= GetLength()) ? Eldritch2::EqualityCompareString( AsCString(), needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::BeginsWith( CharacterType character ) const {
		return (*this)[0] == character;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::EndsWith( const CharacterType* const needle ) const {
		const auto needleLength( Eldritch2::StringLength( needle ) );

		return (needleLength <= GetLength()) ? Eldritch2::EqualityCompareString( End() - needleLength, needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::EndsWith( const CharacterType character ) const {
		return (*this) ? (End()[-1] == character) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::Erase( ConstIterator position, ConstIterator end ) {
		_underlyingContainer.erase( position, end );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::Erase( ConstIterator position ) {
		return _underlyingContainer.erase( position );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator=( const String<Character, Allocator>& rhs ) {
		_underlyingContainer = rhs._underlyingContainer;
		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator=( const CharacterType* const string ) {
		_underlyingContainer = string;
		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Assign( const CharacterType* const begin, const CharacterType* const end ) {
		_underlyingContainer.assign( begin, end );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Assign( const CharacterType* const string ) {
		_underlyingContainer.assign( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const AlternateCharacter* const begin, const AlternateCharacter* const end ) {
		_underlyingContainer.append_convert( begin, eastl::distance( begin, end ) );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const CharacterType* const begin, const CharacterType* const end ) {
		_underlyingContainer.append( begin, end );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const AlternateCharacter* const string ) {
		_underlyingContainer.append_convert( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const CharacterType* const string ) {
		_underlyingContainer.append( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter, class AlternateAllocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const String<AlternateCharacter, AlternateAllocator>& string ) {
		_underlyingContainer.append_convert( string._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const String<Character, Allocator>& string ) {
		_underlyingContainer.append( string._underlyingContainer );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( CharacterType character ) {
		_underlyingContainer.push_back( character );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::EnsureEndsWith( const CharacterType* const string ) {
		if( !EndsWith( string ) ) {
			Append( string );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::EnsureEndsWith( CharacterType character ) {
		if( !EndsWith( character ) ) {
			Append( character );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter, class AlternateAllocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator+=( const String<AlternateCharacter, AlternateAllocator>& rhs ) {
		_underlyingContainer.append_convert( rhs.Begin(), rhs.GetLength() );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator+=( const String<Character, Allocator>& rhs ) {
		_underlyingContainer += rhs._underlyingContainer;

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator+=( const AlternateCharacter* const string ) {
		_underlyingContainer.append_convert( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator+=( const CharacterType* const string ) {
		_underlyingContainer += string;

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator+=( CharacterType character ) {
		_underlyingContainer.push_back( character );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Trim( SizeType charactersToAdvanceBeginning, SizeType charactersToRemoveFromEnd ) {
		_underlyingContainer.erase( _underlyingContainer.end() - charactersToRemoveFromEnd, _underlyingContainer.end() );
		_underlyingContainer.erase( _underlyingContainer.begin(), _underlyingContainer.begin() + charactersToAdvanceBeginning );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Trim( ConstIterator newBegin, ConstIterator newEnd ) {
		_underlyingContainer.erase( newEnd, _underlyingContainer.end() );
		_underlyingContainer.erase( _underlyingContainer.begin(), newBegin );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Resize( const SizeType size ) {
		_underlyingContainer.resize( size );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint int String<Character, Allocator>::Compare( const CharacterType* const string ) const {
		return Eldritch2::CompareString( this->AsCString(), string );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint int String<Character, Allocator>::Compare( const String<Character, Allocator>& rhs ) const {
		return Eldritch2::CompareString( this->AsCString(), rhs.AsCString() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::CharacterType String<Character, Allocator>::operator[]( SizeType indexInCharacters ) const {
		return _underlyingContainer[indexInCharacters];
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::operator typename const String<Character, Allocator>::CharacterType*() const {
		return _underlyingContainer.c_str();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint const typename String<Character, Allocator>::CharacterType* String<Character, Allocator>::AsCString() const {
		return _underlyingContainer.c_str();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::SizeType String<Character, Allocator>::GetLength() const {
		return _underlyingContainer.length();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::IsEmpty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::operator bool() const {
		return !this->IsEmpty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Reserve( SizeType capacityInCharacters ) {
		_underlyingContainer.reserve( capacityHintInBytes );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::SizeType String<Character, Allocator>::GetCapacityInCharacters() const {
		return _underlyingContainer.capacity();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint const typename String<Character, Allocator>::AllocatorType& String<Character, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator==( const String<Character, StringAllocator>& string0, const String<Character, StringAllocator>& string1 ) {
		return 0 == string0.Compare( string1 );
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator==( const String<Character, StringAllocator>& string0, const Character* const string1 ) {
		return 0 == string0.Compare( string1 );
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator==( const Character* const string0, const String<Character, StringAllocator>& string1 ) {
		return 0 == string1.Compare( string0 );
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator!=( const String<Character, StringAllocator>& string0, const String<Character, StringAllocator>& string1 ) {
		return 0 != string0.Compare( string1 );
	}
	
// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator!=( const String<Character, StringAllocator>& string0, const Character* const string1 ) {
		return 0 != string0.Compare( string1 );
	}
	
// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator!=( const Character* const string0, const String<Character, StringAllocator>& string1 ) {
		return 0 != string1.Compare( string0 );
	}

// ---------------------------------------------------

	template <class Stream, typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint Stream& operator>>( Stream& stream, String<Character, StringAllocator>& string0 ) {
		using StreamSentry = Stream::sentry;

	// ---

		StreamSentry	sentry( stream );

		if( sentry ) {

		}

		return stream;
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint size_t GetHashCode( const String<Character, StringAllocator>& string, size_t hashSeed ) {
		return Eldritch2::HashMemory( string.AsCString(), string.GetLength() * sizeof( Character ), hashSeed );
	}

}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#if defined( POP_CompareString )
#	pragma pop_macro( "CompareString" )
#	undef POP_CompareString
#	endif
#endif