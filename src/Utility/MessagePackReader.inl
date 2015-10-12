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

	template <typename Container, typename ElementProvider>
	MessagePackReader::ArrayProxy<Container, ElementProvider>::ArrayProxy( Container& container, ElementProvider elementProvider ) : MessagePackBase::ArrayHeader(), _container( container ), _elementProvider( elementProvider ) {}

// ---------------------------------------------------

	template <typename Container, typename ElementProvider>
	bool MessagePackReader::ArrayProxy<Container, ElementProvider>::Serialize( MessagePackReader& reader ) {
		if( !ArrayHeader::Serialize( reader ) ) {
			return false;
		}

		for( auto currentElement( _container.End() ); 0u != sizeInElements; --sizeInElements ) {
			auto&&	deserializingValue( _elementProvider() );

			if( !reader( deserializingValue ) ) {
				return false;
			}
			
			currentElement = _container.Insert( currentElement, deserializingValue );
		}

		return true;
	}

// ---------------------------------------------------

	template <typename Container, typename ElementProvider, typename KeyExtractor, typename ValueExtractor>
	MessagePackReader::MapProxy<Container, ElementProvider, KeyExtractor, ValueExtractor>::MapProxy( Container&			container,
																									 ElementProvider	elementProvider,
																									 KeyExtractor		keyExtractor,
																									 ValueExtractor		valueExtractor ) : MessagePackBase::MapHeader(),
																																		   _container( container ),
																																		   _elementProvider( elementProvider ),
																																		   _keyExtractor( keyExtractor ),
																																		   _valueExtractor( valueExtractor ) {}

// ---------------------------------------------------

	template <typename Container, typename KeyExtractor, typename ValueExtractor, typename ElementProvider>
	bool MessagePackReader::MapProxy<Container, KeyExtractor, ValueExtractor, ElementProvider>::Serialize( MessagePackReader& reader ) {
		if( !MapHeader::Serialize( reader ) ) {
			return false;
		}

		for( ; 0u != sizeInPairs; --sizeInPairs ) {
			auto&&	deserializingValue( _elementProvider() );

			if( !reader( _keyExtractor( deserializingValue ), _valueExtractor( deserializingValue ) ) ) {
				return false;
			}

			_container.Insert( deserializingValue );
		}

		return true;
	}

// ---------------------------------------------------

	template <typename Container, typename ElementProvider, typename KeyExtractor, typename ValueExtractor>
	MessagePackReader::MapProxy<Container, ElementProvider, KeyExtractor, ValueExtractor>&& MessagePackReader::AdaptMap( Container& container, ElementProvider&& elementProvider, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor ) {
		return { container, ::std::forward<ElementProvider>( elementProvider ), ::std::forward<KeyExtractor>( keyExtractor ), ::std::forward<ValueExtractor>( valueExtractor ) };
	}

// ---------------------------------------------------

	template <typename Container, typename ElementProvider>
	MessagePackReader::ArrayProxy<Container, ElementProvider>&& MessagePackReader::AdaptArray( Container& container, ElementProvider&& elementProvider ) {
		return { container, ::std::forward<ElementProvider>( elementProvider ) };
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackReader::ParseOneOf( size_t&& indexOfUsedElement, Fields&&... fields ) {
		static_assert( 1 < sizeof...(fields), "MessagePackReader::ParseOneOf() requires at least two fields to choose from" );

	// ---

		return TryParseOneOf( indexOfUsedElement, ::std::forward<Fields>( fields )... );
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackReader::operator()( Fields&&... fields ) {
		static_assert( sizeof...(fields) > 0u, "MessagePackReader::operator() requires at least one field!" );

	// ---

		return Parse( fields... );
	}

// ---------------------------------------------------

	template <typename Head, typename... Tail>
	bool MessagePackReader::Parse( Head& head, Tail&... tail ) {
		return Parse( head ) ? Parse( tail... ) : false;
	}

// ---------------------------------------------------

	template <typename Head, typename... Fields>
	ETInlineHint bool MessagePackReader::TryParseOneOf( size_t&& indexOfUsedElement, Head&& head, Fields&&... fields ) {
		const auto	checkpoint( CreateCheckpoint() );

		if( Parse( head ) ) {
			indexOfUsedElement = sizeof...(fields);
			return true;
		}

		// We encountered the wrong type. This really should have been in the header, but whatever.
		if( 7 == error ) {
			// Didn't parse correctly. Revert changes.
			RestoreCheckpoint( checkpoint );

			return TryParseOneOf( indexOfUsedElement, fields... );
		}

		return false;
	}
	
// ---------------------------------------------------
	
	template <typename Head>
	ETInlineHint bool MessagePackReader::TryParseOneOf( size_t&& indexOfUsedElement, Head&& head ) {
		const auto	checkpoint( CreateCheckpoint() );

		if( Parse( head ) ) {
			indexOfUsedElement = 0u;
			return true;
		}

		RestoreCheckpoint( checkpoint );

		return false;
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<bool>( bool& element ) {
		return ::cmp_read_bool( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::uint8>( ::Eldritch2::uint8& element ) {
		return ::cmp_read_uchar( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::int8>( ::Eldritch2::int8& element ) {
		return ::cmp_read_char( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::uint16>( ::Eldritch2::uint16& element ) {
		return ::cmp_read_ushort( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::int16>( ::Eldritch2::int16& element ) {
		return ::cmp_read_short( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::uint32>( ::Eldritch2::uint32& element ) {
		return ::cmp_read_uint( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::int32>( ::Eldritch2::int32& element ) {
		return ::cmp_read_int( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::uint64>( ::Eldritch2::uint64& element ) {
		return ::cmp_read_ulong( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::int64>( ::Eldritch2::int64& element ) {
		return ::cmp_read_long( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::float64>( ::Eldritch2::float64& element ) {
		return ::cmp_read_double( this, &element );
	}

// ---------------------------------------------------

	template <>
	ETInlineHint bool MessagePackReader::Parse<::Eldritch2::float32>( ::Eldritch2::float32& element ) {
		return ::cmp_read_float( this, &element );
	}

// ---------------------------------------------------

	template <typename Head>
	ETInlineHint bool MessagePackReader::Parse( Head& element ) {
		return element.Serialize( *this );
	}

}	// namespace Utility
}	// namespace Eldritch2