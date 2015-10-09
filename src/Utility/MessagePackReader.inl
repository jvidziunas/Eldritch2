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
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::Nil>( MessagePackReader::Nil& /*element*/ ) const {
		return ::cmp_object_is_nil( const_cast<DynamicElement*>(this) );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::ArrayHeader>( MessagePackReader::ArrayHeader& element ) const {		
		return ::cmp_object_as_array( const_cast<DynamicElement*>(this), &element.arraySizeInElements );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::MapHeader>( MessagePackReader::MapHeader& element ) const {		
		return ::cmp_object_as_map( const_cast<DynamicElement*>(this), &element.mapSizeInPairs );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::ObjectReference>( MessagePackReader::ObjectReference& element ) const {
		::Eldritch2::uint32	extendedDataSize;
		::Eldritch2::int8	extendedDataType;
		
		if( ::cmp_object_as_ext( const_cast<DynamicElement*>(this), &extendedDataType, &extendedDataSize ) && (KnownExtendedDataMarkers::OBJECT_REFERENCE == extendedDataType) && (extendedDataSize <= sizeof( element.identifier )) ) {
			_reader.ReadRaw( &element.identifier, extendedDataSize );
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::InPlaceString>( MessagePackReader::InPlaceString& element ) const {
		::Eldritch2::uint32	stringSizeInOctets;

		return ::cmp_object_as_str( const_cast<DynamicElement*>(this), &stringSizeInOctets ) ? _reader.GetInPlaceDataSpan( stringSizeInOctets, element ) : false;
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::InPlaceBulkData>( MessagePackReader::InPlaceBulkData& element ) const {
		::Eldritch2::uint32	sizeInBytes;

		return ::cmp_object_as_bin( const_cast<DynamicElement*>(this), &sizeInBytes ) ? _reader.GetInPlaceDataSpan( sizeInBytes, element ) : false;
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::DynamicElement::operator()<MessagePackReader::TypeString>( MessagePackReader::TypeString& element ) const {
		::Eldritch2::uint32	size;
		::Eldritch2::int8	type;

		return ::cmp_object_as_ext( const_cast<DynamicElement*>(this), &type, &size ) && (KnownExtendedDataMarkers::TYPE_STRING == type) ? _reader.GetInPlaceDataSpan( size, element ) : false;
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
		// Using the comma operator to set the offset before returning.
		return (*this)( head ) ? (indexOfUsedElement = sizeof...(tail), true) : ParseOneOf( parsingElement, indexOfUsedElement, ::std::forward<Tail>( tail )... );
	}

// ---------------------------------------------------

	template <typename Head>
	bool MessagePackReader::DynamicElement::ParseOneOf( size_t& indexOfUsedElement, Head&& head ) const {
		// Using the comma operator to set the offset before returning.
		return (*this)(head) ? (indexOfUsedElement = 0u, true) : false;
	}

// ---------------------------------------------------

	ETInlineHint MessagePackReader::InPlaceString::InPlaceString() : ::Eldritch2::Range<const char*>( EmptySet() ) {}

// ---------------------------------------------------

	ETInlineHint MessagePackReader::InPlaceBulkData::InPlaceBulkData() : ::Eldritch2::Range<const char*>( EmptySet() ) {}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider>
	MessagePackReader::ArrayWrapper<Container, DefaultElementProvider>::ArrayWrapper( Container& container, DefaultElementProvider&& defaultElementProvider ) : _container( container ), _defaultElementProvider( defaultElementProvider ) {}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider>
	bool MessagePackReader::ArrayWrapper<Container, DefaultElementProvider>::Serialize( Utility::MessagePackReader& reader ) {
		MessagePackReader::ArrayHeader	header;

		if( !reader( header ) ) {
			return false;
		}

		_container.Reserve( header.arraySizeInElements );
		
		for( 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
			auto&&	value( _defaultElementProvider() );
			if( !reader( value ) ) {
				return false;
			}

			_container.PushBack( value );
		}

		return true;
	}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider, class KeyExtractor, class ValueExtractor>
	MessagePackReader::MapWrapper<Container, DefaultElementProvider, KeyExtractor, ValueExtractor>::MapWrapper( Container& container, DefaultElementProvider&& defaultElementProvider, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor ) : _container( container ),
																																																														_defaultElementProvider( defaultElementProvider ),
																																																														_keyExtractor( keyExtractor ),
																																																														_valueExtractor( valueExtractor ) {}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider, class KeyExtractor, class ValueExtractor>
	bool MessagePackReader::MapWrapper<Container, DefaultElementProvider, KeyExtractor, ValueExtractor>::Serialize( Utility::MessagePackReader& reader ) {
		MessagePackReader::MapHeader	header;

		if( !reader( header ) ) {
			return false;
		}

		for( ; 0u != header.arraySizeInElements; --header.arraySizeInElements ) {
			auto&&	value( _defaultElementProvider() );

			if( !reader( _keyExtractor( value ), _valueExtractor( value ) ) ) {
				return false;
			}

			_container.Insert( value );
		}

		return true;
	}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider>
	typename MessagePackReader::ArrayWrapper<Container, DefaultElementProvider>&& MessagePackReader::WrapArrayContainer( Container& container, DefaultElementProvider&& elementProvider ) {
		return { container, ::std::forward<DefaultElementProvider>( elementProvider ) };
	}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider, class KeyExtractor, class ValueExtractor>
	typename MessagePackReader::MapWrapper<Container, DefaultElementProvider, KeyExtractor, ValueExtractor>&& MessagePackReader::WrapMapContainer( Container& container, DefaultElementProvider&& elementProvider, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor ) {
		return { container, ::std::forward<DefaultElementProvider>( elementProvider ), ::std::forward<KeyExtractor>( keyExtractor ), ::std::forward<ValueExtractor>( valueExtractor ) };
	}

// ---------------------------------------------------

	template <class Container, class DefaultElementProvider>
	typename MessagePackReader::MapWrapper<Container, DefaultElementProvider>&& MessagePackReader::WrapMapContainer( Container& container, DefaultElementProvider&& elementProvider ) {
		return { container, ::std::forward<DefaultElementProvider>( elementProvider ), MessagePackBase::DefaultKeyExtractor<Container>(), MessagePackBase::DefaultValueExtractor<Container>() };
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackReader::ParseOneOf( size_t&& indexOfUsedElement, Fields&&... fields ) {
		static_assert( 1 < sizeof...(fields), "MessagePackReader::ParseOneOf() requires at least two fields to choose from" );

		return DynamicElement( *this ).ParseOneOf( indexOfUsedElement, ::std::forward<Fields>( fields )... );
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackReader::operator()( Fields&&... fields ) {
		static_assert( sizeof...(fields) > 0u, "MessagePackReader::operator() requires at least one field!" );

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