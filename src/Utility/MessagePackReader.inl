/*==================================================================*\
  MessagePackReader.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	ETInlineHint MessagePackReader::DynamicElement::DynamicElement( MessagePackReader& reader ) : _reader( reader ) {
		::cmp_read_object( &reader, this );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<bool>( bool& element ) const {
		return ::cmp_object_as_bool( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::uint8>( ::Eldritch2::uint8& element ) const {
		return ::cmp_object_as_uchar( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::int8>( ::Eldritch2::int8& element ) const {
		return ::cmp_object_as_char( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::uint16>( ::Eldritch2::uint16& element ) const {
		return ::cmp_object_as_ushort( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::int16>( ::Eldritch2::int16& element ) const {
		return ::cmp_object_as_short( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::uint32>( ::Eldritch2::uint32& element ) const {
		return ::cmp_object_as_uint( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::int32>( ::Eldritch2::int32& element ) const {
		return ::cmp_object_as_int( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::uint64>( ::Eldritch2::uint64& element ) const {
		return ::cmp_object_as_ulong( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::int64>( ::Eldritch2::int64& element ) const {
		return ::cmp_object_as_long( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::float64>( ::Eldritch2::float64& element ) const {
		return ::cmp_object_as_double( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<::Eldritch2::float32>( ::Eldritch2::float32& element ) const {
		return ::cmp_object_as_float( const_cast<DynamicElement*>(this), &element );
	}

// ---------------------------------------------------

	template <typename ElementToParse>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()( ElementToParse& element ) const {
		static_assert( !::std::is_same<ElementToParse, DynamicElement>::value, "Dynamic objects must be parsed into a concrete type!" );

		return element.Serialize( _reader );
	}

// ---------------------------------------------------

	template <typename Head, typename... Tail>
	bool MessagePackReader::DynamicElement::ParseOneOf( size_t& indexOfUsedElement, Head&& head, Tail&&... tail ) const {
		const auto	checkpoint( CreateCheckpoint() );
		const bool	parseResult( (*this)(head) );

		if( parseResult ) {
			indexOfUsedElement = sizeof...(tail);
		} else {
			// Didn't parse correctly. Revert changes, if any, and try again with the next element type.
			RestoreCheckpoint( checkpoint );
		}
		
		return parseResult ? true : ParseOneOf( indexOfUsedElement, ::std::forward<Tail>( tail )... );
	}

// ---------------------------------------------------

	template <typename Head>
	bool MessagePackReader::DynamicElement::ParseOneOf( size_t& indexOfUsedElement, Head&& head ) const {
		const auto	checkpoint( CreateCheckpoint() );
		const bool	parseResult( (*this)(head) );

		if( parseResult ) {
			indexOfUsedElement = 0u;
		} else {
			// Didn't parse correctly. Revert changes, if any.
			RestoreCheckpoint( checkpoint );
		}

		return parseResult;
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackReader::ParseOneOf( size_t&& indexOfUsedElement, Fields&&... fields ) {
		static_assert( 1 < sizeof...(fields), "MessagePackReader::ParseOneOf() requires at least two fields to choose from" );

	// ---

		const auto	checkpoint( CreateCheckpoint() );
		const bool	parseResult( DynamicElement( *this ).ParseOneOf( indexOfUsedElement, ::std::forward<Fields>( fields )... ) );

		if( !parseResult ) {
			// Didn't parse correctly. Revert changes.
			RestoreCheckpoint( checkpoint );
		}

		return parseResult;
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackReader::operator()( Fields&&... fields ) {
		static_assert( sizeof...(fields) > 0u, "MessagePackReader::operator() requires at least one field!" );

	// ---

		return Parse( ::std::forward<Fields>( fields )... );
	}

// ---------------------------------------------------

	template <typename Head, typename... Tail>
	bool MessagePackReader::Parse( Head&& head, Tail&&... tail ) {
		return Parse( ::std::forward<Head>( head ) ) ? Parse( ::std::forward<Tail>( tail )... ) : false;
	}

// ---------------------------------------------------

	template <typename Head>
	bool MessagePackReader::Parse( Head&& head ) {
		return DynamicElement( *this )( head );
	}

}	// namespace Utility
}	// namespace Eldritch2