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
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//
#include <utfcpp/v2_0/source/utf8/unchecked.h>
//------------------------------------------------------------------//
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Allocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( const EmptyStringSemantics, AllocatorType&& allocator ) : UTF8String<Allocator>( "", ::std::move( allocator ) ) {}

// ---------------------------------------------------
	
	template <class Allocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( const CharacterType* const string, const CharacterType* const stringEnd, AllocatorType&& allocator ) : _underlyingContainer( string, static_cast<SizeType>(stringEnd - string), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	template <size_t literalLength>
	// Remember to subtract 1 here to account for the terminating null character.
	ETInlineHint UTF8String<Allocator>::UTF8String( const CharacterType (&stringLiteral)[literalLength], AllocatorType&& allocator ) : _underlyingContainer( stringLiteral, static_cast<SizeType>(literalLength-1), ::std::move( allocator ) ) {}
	
// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint UTF8String<Allocator>::UTF8String( const ::Eldritch2::UTF8String<AlternateAllocator>& string, AllocatorType&& allocator ) : UTF8String<Allocator>( string.Begin(), string.End(), ::std::move( allocator ) ) {}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::IteratorToFirstInstance( const CharacterType character ) const {
		return ::Eldritch2::FindFirstInstance( GetCharacterArray(), character );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::IteratorToFirstInstance( const CharacterType* const needle ) const {
		return ::Eldritch2::FindFirstInstance( GetCharacterArray(), needle );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::IteratorToFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		auto	position( IteratorToFirstInstance( needle ) );

		return position ? position + ::Eldritch2::StringLength( needle ) : nullptr;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::IteratorToLastInstance( const CharacterType character ) const {
		return ::Eldritch2::FindLastInstance( GetCharacterArray(), character );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::IteratorToLastInstance( const CharacterType* const needle ) const {
		return ::Eldritch2::FindLastInstance( GetCharacterArray(), needle );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::IteratorToLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		auto	position( IteratorToFirstInstance( needle ) );

		return position ? position + ::Eldritch2::StringLength( needle ) : nullptr;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::Contains( const CharacterType character ) const {
		return End() != IteratorToFirstInstance( character );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::Contains( const CharacterType* const needle ) const {
		return End() != tIteratorToFirstInstance( needle );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint void UTF8String<Allocator>::MakeLower() {
		_underlyingContainer.make_lower();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint void UTF8String<Allocator>::MakeUpper() {
		_underlyingContainer.make_upper();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::BeginsWith( const CharacterType character ) const {
		return (*this)[0] == character;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::BeginsWith( const CharacterType* const needle ) const {
		const auto needleLength( ::Eldritch2::StringLength( needle ) );

		return (needleLength <= Length()) ? ::Eldritch2::EqualityCompareString( GetCharacterArray(), needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::EndsWith( const CharacterType character ) const {
		return (*this) ? End()[-1] == character : false;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::EndsWith( const CharacterType* const needle ) const {
		const auto needleLength( ::Eldritch2::StringLength( needle ) );

		return (needleLength <= Length()) ? ::Eldritch2::EqualityCompareString( End() - needleLength, needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::Begin() const {
		return _underlyingContainer.begin();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::ConstIterator UTF8String<Allocator>::End() const {
		return _underlyingContainer.end();
	}

// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::operator=( const ::Eldritch2::UTF8String<AlternateAllocator>& rhs ) {
		return Assign( rhs.Begin(), rhs.LengthInBytes() );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::operator=( const CharacterType* const string ) {
		return Assign( string );
	}

// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint ::Eldritch2::UTF8String<AlternateAllocator> UTF8String<Allocator>::Substring( AlternateAllocator& allocator, const ConstIterator begin, const ConstIterator end ) {
		return { begin, end, allocator, UTF8L("String Allocator") };
	}

// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint ::Eldritch2::UTF8String<AlternateAllocator> UTF8String<Allocator>::Substring( AlternateAllocator& allocator, const ConstIterator begin ) {
		return Substring( allocator, begin, this->End() );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::Assign( const CharacterType* const string, const SizeType lengthInBytes ) {
		_underlyingContainer.assign( string, lengthInBytes );

		return *this;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::Assign( const CharacterType* const string ) {
		_underlyingContainer.assign( string, ::Eldritch2::StringLength( string ) );

		return *this;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::Append( const CharacterType character ) {
		_underlyingContainer.append( character );

		return *this;
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::Append( const CharacterType* const string ) {
		return Append( string, static_cast<SizeType>(::Eldritch2::StringLength( string )) );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::Append( const CharacterType* const stringBegin, const SizeType lengthInBytes ) {
		_underlyingContainer.append( stringBegin, lengthInBytes );

		return *this;
	}

// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::Append( const ::Eldritch2::UTF8String<AlternateAllocator>& string ) {
		return Append( string.Begin(), string.Length() );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::EnsureEndsWith( const CharacterType character ) {
		if( !EndsWith( character ) ) {
			Append( character );
		}
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::EnsureEndsWith( const CharacterType* const string ) {
		if( !EndsWith( string ) ) {
			Append( string );
		}
	}

// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint ::Eldritch2::UTF8String<Allocator>& UTF8String<Allocator>::operator+=( const ::Eldritch2::UTF8String<AlternateAllocator>& rhs ) {
		return Append( rhs );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint void UTF8String<Allocator>::Clear() {
		_underlyingContainer.clear();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint void UTF8String<Allocator>::Resize( const SizeType size ) {
		_underlyingContainer.resize( size );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint int UTF8String<Allocator>::Compare( const CharacterType* const string ) const {
		return ::Eldritch2::CompareString( GetCharacterArray(), string );
	}

// ---------------------------------------------------

	template <class Allocator>
	template <class AlternateAllocator>
	ETInlineHint int UTF8String<Allocator>::Compare( const ::Eldritch2::UTF8String<AlternateAllocator>& rhs ) const {
		return ::Eldritch2::CompareString( GetCharacterArray(), rhs.GetCharacterArray() );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::CharacterType UTF8String<Allocator>::operator[]( const SizeType indexInBytes ) const {
		return _underlyingContainer[indexInBytes];
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename const UTF8String<Allocator>::CharacterType* UTF8String<Allocator>::GetCharacterArray() const {
		return _underlyingContainer.c_str();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::SizeType UTF8String<Allocator>::Length() const {
		return static_cast<SizeType>(::std::distance( _underlyingContainer.begin(), _underlyingContainer.end() ));
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::SizeType UTF8String<Allocator>::Length( const UTF32CodepointSemantics ) const {
		return static_cast<SizeType>(::utf8::unchecked::distance( _underlyingContainer.begin(), _underlyingContainer.end() ));
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint bool UTF8String<Allocator>::Empty() const {
		return _underlyingContainer.empty();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint UTF8String<Allocator>::operator bool() {
		return !Empty();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint void UTF8String<Allocator>::Reserve( const SizeType capacityHintInBytes ) {
		_underlyingContainer.reserve( capacityHintInBytes );
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename UTF8String<Allocator>::SizeType UTF8String<Allocator>::CapacityInBytes() const {
		return _underlyingContainer.capacity();
	}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint typename const UTF8String<Allocator>::AllocatorType& UTF8String<Allocator>::GetAllocator() const {
		return _underlyingContainer.get_allocator();
	}

}	// namespace Eldritch2