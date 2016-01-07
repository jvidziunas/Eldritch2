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
#include <Utility/Hash.hpp>
//------------------------------------------------------------------//
#include <utfcpp/v2_0/source/utf8/unchecked.h>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
//------------------------------------------------------------------//
#include <type_traits>
//------------------------------------------------------------------//

#if defined( CompareString )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_CompareString 1
		COMPILERMESSAGEGENERIC( "Cleaning up CompareString macro for use in ::Eldritch2::String." )
#		pragma push_macro( "CompareString" )
#		undef CompareString
#	else
#		error CompareString macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif

namespace Eldritch2 {
	
	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( AllocatorType&& allocator ) : _underlyingContainer( ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator ) : String( ::std::move( allocator ) ) {
		Assign( string, stringEnd );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <size_t literalLength>
	// Remember to subtract 1 here to account for the terminating null character.
	ETInlineHint String<Character, Allocator>::String( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator ) : _underlyingContainer( stringLiteral, static_cast<SizeType>(literalLength-1), ::std::move( allocator ) ) {}
	
// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, Allocator>::String( const String<Character, AlternateAllocator>& string, AllocatorType&& allocator ) : String<Character, Allocator>( string.Begin(), string.End(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( String<Character, Allocator>&& sourceString ) : _underlyingContainer( ::std::move( sourceString._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::IteratorToFirstInstance( const CharacterType character ) const {
		return ::Eldritch2::FindFirstInstance( GetCharacterArray(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::IteratorToFirstInstance( const CharacterType* const needle ) const {
		return ::Eldritch2::FindFirstInstance( GetCharacterArray(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::IteratorToFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		auto	position( IteratorToFirstInstance( needle ) );

		return position ? position + ::Eldritch2::StringLength( needle ) : nullptr;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::IteratorToLastInstance( const CharacterType character ) const {
		return ::Eldritch2::FindLastInstance( GetCharacterArray(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::IteratorToLastInstance( const CharacterType* const needle ) const {
		return ::Eldritch2::FindLastInstance( GetCharacterArray(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::IteratorToLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		auto	position( IteratorToFirstInstance( needle ) );

		return position ? position + ::Eldritch2::StringLength( needle ) : nullptr;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Contains( const CharacterType character ) const {
		return End() != IteratorToFirstInstance( character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Contains( const CharacterType* const needle ) const {
		return End() != tIteratorToFirstInstance( needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::MakeLower() {
		_underlyingContainer.make_lower();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::MakeUpper() {
		_underlyingContainer.make_upper();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::BeginsWith( const CharacterType character ) const {
		return (*this)[0] == character;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::BeginsWith( const CharacterType* const needle ) const {
		const auto needleLength( ::Eldritch2::StringLength( needle ) );

		return (needleLength <= Length()) ? ::Eldritch2::EqualityCompareString( GetCharacterArray(), needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::EndsWith( const CharacterType character ) const {
		return (*this) ? (End()[-1] == character) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::EndsWith( const CharacterType* const needle ) const {
		const auto needleLength( ::Eldritch2::StringLength( needle ) );

		return (needleLength <= Length()) ? ::Eldritch2::EqualityCompareString( End() - needleLength, needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename Value>
	bool String<Character, Allocator>::ParseInto( Value&& value ) const {
		using namespace ::boost::iostreams;

	// ---

		basic_array_source<CharacterType>	source( Begin(), End() );
		stream<decltype(source)>			inStream( source );

		return (inStream >> value).fail();
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
	template <class AlternateAllocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator=( const String<Character, AlternateAllocator>& rhs ) {
		return Assign( rhs.Begin(), rhs.End() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator=( const CharacterType* const string ) {
		return Assign( string );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, AlternateAllocator> String<Character, Allocator>::Substring( AlternateAllocator& allocator, const ConstIterator begin, const ConstIterator end ) {
		return { begin, end, allocator, UTF8L("String Allocator") };
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, AlternateAllocator> String<Character, Allocator>::Substring( AlternateAllocator& allocator, const ConstIterator begin ) {
		return Substring( allocator, begin, this->End() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Assign( const CharacterType* const begin, const CharacterType* const end ) {
		_underlyingContainer.assign( begin, end - begin );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Assign( const CharacterType* const string ) {
		_underlyingContainer.assign( string, ::Eldritch2::FindEndOfString( string ) );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const CharacterType character ) {
		_underlyingContainer.append( character );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const CharacterType* const string ) {
		return Append( string, ::Eldritch2::FindEndOfString( string ) );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const CharacterType* const begin, const CharacterType* const end ) {
		_underlyingContainer.append( begin, end - begin );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const String<Character, AlternateAllocator>& string ) {
		return Append( string.Begin(), string.Length() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::EnsureEndsWith( const CharacterType character ) {
		if( !EndsWith( character ) ) {
			Append( character );
		}
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::EnsureEndsWith( const CharacterType* const string ) {
		if( !EndsWith( string ) ) {
			Append( string );
		}
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::operator+=( const String<Character, AlternateAllocator>& rhs ) {
		return Append( rhs );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Resize( const SizeType size ) {
		_underlyingContainer.resize( size );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint int String<Character, Allocator>::Compare( const CharacterType* const string ) const {
		return ::Eldritch2::CompareString( GetCharacterArray(), string );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint int String<Character, Allocator>::Compare( const String<Character, AlternateAllocator>& rhs ) const {
		return ::Eldritch2::CompareString( GetCharacterArray(), rhs.GetCharacterArray() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint size_t	String<Character, Allocator>::GetHashCode( const size_t hashSeed ) const {
		return ::Eldritch2::HashMemory( GetCharacterArray(), (Length() * sizeof(CharacterType)), hashSeed );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::CharacterType String<Character, Allocator>::operator[]( const SizeType indexInBytes ) const {
		return _underlyingContainer[indexInBytes];
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint const typename String<Character, Allocator>::CharacterType* String<Character, Allocator>::GetCharacterArray() const {
		return _underlyingContainer.c_str();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::SizeType String<Character, Allocator>::Length() const {
		return static_cast<SizeType>(::std::distance( _underlyingContainer.begin(), _underlyingContainer.end() ));
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::operator bool() {
		return !Empty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Reserve( const SizeType capacityHintInBytes ) {
		_underlyingContainer.reserve( capacityHintInBytes );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::SizeType String<Character, Allocator>::CapacityInBytes() const {
		return _underlyingContainer.capacity();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint const typename String<Character, Allocator>::AllocatorType& String<Character, Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator0, class StringAllocator1>
	ETNoAliasHint bool operator==( const String<Character, StringAllocator0>& string0, const String<Character, StringAllocator1>& string1 ) {
		return 0 == string0.Compare( string1 );
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETNoAliasHint bool operator==( const String<Character, StringAllocator>& string0, const Character* const string1 ) {
		return 0 == string0.Compare( string1 );
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETNoAliasHint bool operator==( const Character* const string0, const String<Character, StringAllocator>& string1 ) {
		return 0 == string1.Compare( string0 );
	}

// ---------------------------------------------------

	template <class Stream, typename Character, class StringAllocator>
	ETNoAliasHint Stream& operator>>( Stream& stream, String<Character, StringAllocator>& string0 ) {
		using StreamSentry = Stream::sentry;

	// ---

		StreamSentry	sentry( stream );

		if( sentry ) {

		}

		return stream;
	}

}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#if defined( POP_CompareString )
#	pragma pop_macro( "CompareString" )
#	undef POP_CompareString
#	endif
#endif