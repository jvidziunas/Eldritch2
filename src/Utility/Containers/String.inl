/*==================================================================*\
  String.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
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
	ETInlineHint String<Character, Allocator>::String( const CharacterType* const string, AllocatorType&& allocator = AllocatorType() ) : String( string, ::Eldritch2::FindEndOfString( string ), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator ) : _underlyingContainer( string, static_cast<SizeType>(stringEnd - string), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <size_t literalLength>
	// Remember to subtract 1 here to account for the terminating null character.
	ETInlineHint String<Character, Allocator>::String( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator ) : _underlyingContainer( stringLiteral, static_cast<SizeType>(literalLength)-1, ::std::move( allocator ) ) {}
	
// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, Allocator>::String( const String<Character, AlternateAllocator>& string, AllocatorType&& allocator ) : String<Character, Allocator>( string.Begin(), string.End(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>::String( String<Character, Allocator>&& sourceString ) : _underlyingContainer( ::std::move( sourceString._underlyingContainer ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( CharacterType character ) const {
		return FindFirstInstance( Begin(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( const CharacterType* const needle ) const {
		return FindFirstInstance( Begin(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics semantics ) const {
		return FindFirstInstance( Begin(), needle, semantics );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( ConstIterator startPosition, CharacterType character ) const {
		const auto	position( ::Eldritch2::FindFirstInstance( startPosition, character ) );

		return position ? position : End();
	}

// ---------------------------------------------------
	
	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( ConstIterator startPosition, const CharacterType* const needle ) const {
		const auto	position( ::Eldritch2::FindFirstInstance( startPosition, needle ) );

		return position ? position : End();
	}

// ---------------------------------------------------
	
	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindFirstInstance( ConstIterator startPosition, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		auto	position( ::Eldritch2::FindFirstInstance( startPosition, needle ) );

		return position ? position + ::Eldritch2::StringLength( needle ) : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( CharacterType character ) const {
		return FindLastInstance( Begin(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( const CharacterType* const needle ) const {
		return FindLastInstance( Begin(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics semantics ) const {
		return FindLastInstance( Begin(), needle, semantics );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( ConstIterator startPosition, CharacterType character ) const {
		const auto	position( ::Eldritch2::FindLastInstance( startPosition, character ) );

		return position ? position : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( ConstIterator startPosition, const CharacterType* const needle ) const {
		const auto	position( ::Eldritch2::FindLastInstance( startPosition, needle ) );

		return position ? position : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::ConstIterator String<Character, Allocator>::FindLastInstance( ConstIterator startPosition, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		const auto	position( ::Eldritch2::FindLastInstance( startPosition, needle ) );

		return position ? position + ::Eldritch2::StringLength( needle ) : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Contains( const CharacterType character ) const {
		return nullptr != FindFirstInstance( character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool String<Character, Allocator>::Contains( const CharacterType* const needle ) const {
		return nullptr != FindFirstInstance( needle );
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

		return (needleLength <= GetLength()) ? ::Eldritch2::EqualityCompareString( AsCString(), needle, needleLength ) : false;
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

		return (needleLength <= GetLength()) ? ::Eldritch2::EqualityCompareString( End() - needleLength, needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Erase( ConstIterator position, SizeType characterCount ) {
		if( position != End() ) {
			_underlyingContainer.erase( position, characterCount );
		}
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Erase( ConstIterator position, ConstIterator end ) {
		if( position != End() ) {
			_underlyingContainer.erase( position, end );
		}
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename Value>
	bool String<Character, Allocator>::ParseInto( Value&& value ) const {
		::boost::iostreams::basic_array_source<CharacterType>	source( Begin(), End() );
		::boost::iostreams::stream<decltype(source)>			inStream( source );

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
	ETInlineHint String<Character, AlternateAllocator> String<Character, Allocator>::CreateSubstring( AlternateAllocator&& allocator, const ConstIterator begin, const ConstIterator end ) {
		return { begin, end, ::std::move( allocator ) };
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, AlternateAllocator> String<Character, Allocator>::CreateSubstring( AlternateAllocator&& allocator, const ConstIterator begin ) {
		return CreateSubstring( ::std::move( allocator ), begin, this->End() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Assign( const CharacterType* const begin, const CharacterType* const end ) {
		_underlyingContainer.assign( begin, static_cast<SizeType>(end - begin) );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Assign( const CharacterType* const string ) {
		return Assign( string, ::Eldritch2::FindEndOfString( string ) );
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
		_underlyingContainer.append( begin, static_cast<SizeType>(end - begin) );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::Append( const String<Character, AlternateAllocator>& string ) {
		return Append( string.Begin(), string.End() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint String<Character, Allocator>& String<Character, Allocator>::EnsureEndsWith( const CharacterType character ) {
		if( !EndsWith( character ) ) {
			Append( character );
		}

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
	ETInlineHint void String<Character, Allocator>::Trim( const SizeType charactersToAdvanceBeginning, const SizeType charactersToRemoveFromEnd ) {
		_underlyingContainer.trim( charactersToAdvanceBeginning, charactersToRemoveFromEnd );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Trim( ConstIterator newBegin, ConstIterator newEnd ) {
		_underlyingContainer.trim( newBegin, newEnd );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Resize( const SizeType size ) {
		_underlyingContainer.resize( size );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint int String<Character, Allocator>::Compare( const CharacterType* const string ) const {
		return ::Eldritch2::CompareString( AsCString(), string );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <class AlternateAllocator>
	ETInlineHint int String<Character, Allocator>::Compare( const String<Character, AlternateAllocator>& rhs ) const {
		return ::Eldritch2::CompareString( AsCString(), rhs.AsCString() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint size_t	String<Character, Allocator>::GetHashCode( const size_t hashSeed ) const {
		return ::Eldritch2::HashMemory( AsCString(), (GetLength() * sizeof(CharacterType)), hashSeed );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::CharacterType String<Character, Allocator>::operator[]( const SizeType indexInBytes ) const {
		return _underlyingContainer[indexInBytes];
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
	ETInlineHint String<Character, Allocator>::operator bool() {
		return !IsEmpty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void String<Character, Allocator>::Reserve( const SizeType capacityHintInBytes ) {
		_underlyingContainer.reserve( capacityHintInBytes );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename String<Character, Allocator>::SizeType String<Character, Allocator>::GetCapacityInBytes() const {
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

	template <typename Character, class StringAllocator0, class StringAllocator1>
	ETNoAliasHint bool operator!=( const String<Character, StringAllocator0>& string0, const String<Character, StringAllocator1>& string1 ) {
		return 0 != string0.Compare( string1 );
	}
	
// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETNoAliasHint bool operator!=( const String<Character, StringAllocator>& string0, const Character* const string1 ) {
		return 0 != string0.Compare( string1 );
	}
	
// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETNoAliasHint bool operator!=( const Character* const string0, const String<Character, StringAllocator>& string1 ) {
		return 0 != string1.Compare( string0 );
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

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	class Hash<::Eldritch2::String<Character, StringAllocator>> : public ::Eldritch2::Hash<Character*> {
	public:
		Hash() = default;
		~Hash() = default;

	// ---------------------------------------------------

		using ::Eldritch2::Hash<Character*>::operator();

		ETNoAliasHint size_t operator()( const ::Eldritch2::String<Character, StringAllocator>& key, const size_t seed = 0 ) const {
			return key.GetHashCode( seed );
		}
	};

}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#if defined( POP_CompareString )
#	pragma pop_macro( "CompareString" )
#	undef POP_CompareString
#	endif
#endif