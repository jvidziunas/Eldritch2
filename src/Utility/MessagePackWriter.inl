/*==================================================================*\
  MessagePackWriter.inl
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
//------------------------------------------------------------------//
#include <algorithm>
#include <iterator>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	template <class Container>
	MessagePackWriter::ArrayHeaderProxy<Container>::ArrayHeaderProxy( Container& container ) : _container( container ) {}

// ---------------------------------------------------

	template <class Container>
	bool MessagePackWriter::ArrayHeaderProxy<Container>::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_array( &writer, static_cast<::Eldritch2::uint32>(_container.Size()) );
	}

// ---------------------------------------------------

	template <class Container>
	MessagePackWriter::MapHeaderProxy<Container>::MapHeaderProxy( Container& container ) : _container( container ) {}

// ---------------------------------------------------

	template <class Container>
	bool MessagePackWriter::MapHeaderProxy<Container>::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_map( &writer, static_cast<::Eldritch2::uint32>(_container.Size()) );
	}

// ---------------------------------------------------

	template <class Container>
	MessagePackWriter::ArrayProxy<Container>::ArrayProxy( Container& container ) : _container( container ) {}

// ---------------------------------------------------

	template <class Container>
	bool MessagePackWriter::ArrayProxy<Container>::Serialize( Utility::MessagePackWriter& writer ) {
		if( !writer( MessagePackWriter::ArrayHeaderProxy<Container>( _container ) ) ) {
			return false;
		}

		for( const auto& element : _container ) {
			if( !writer( element ) ) {
				return false;
			}
		}

		return true;
	}

// ---------------------------------------------------

	template <class Container, typename KeyExtractor, typename ValueExtractor>
	MessagePackWriter::MapProxy<Container, KeyExtractor, ValueExtractor>::MapProxy( Container& container, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor ) : _container( container ), _keyExtractor( keyExtractor ), _valueExtractor( valueExtractor ) {}

// ---------------------------------------------------

	template <class Container, typename KeyExtractor, typename ValueExtractor>
	bool MessagePackWriter::MapProxy<Container, KeyExtractor, ValueExtractor>::Serialize( Utility::MessagePackWriter& writer ) {
		if( !writer( MessagePackWriter::MapHeaderProxy<Container>( _container ) ) ) {
			return false;
		}

		for( auto& element : _container ) {
			if( !writer( _keyExtractor( element ), _valueExtractor( element ) ) ) {
				return false;
			}
		}

		return true;
	}

// ---------------------------------------------------

	template <class Container, typename Ignored>
	typename MessagePackWriter::ArrayProxy<Container>&& MessagePackWriter::WrapArrayContainer( Container& container, Ignored&& /*ignored*/ ) {
		return { container };
	}

// ---------------------------------------------------

	template <class Container, typename Ignored, class KeyExtractor, class ValueExtractor>
	typename MessagePackWriter::MapProxy<Container, KeyExtractor, ValueExtractor>&& MessagePackWriter::WrapMapContainer( Container& container, Ignored&& /*ignored*/, KeyExtractor&& keyExtractor, ValueExtractor&& valueExtractor ) {
		return { container, ::std::forward<KeyExtractor>( keyExtractor ), ::std::forward<ValueExtractor>( valueExtractor ) };
	}

// ---------------------------------------------------

	template <class Container, typename Ignored>
	typename MessagePackWriter::MapProxy<Container>&& MessagePackWriter::WrapMapContainer( Container& container, Ignored&& /*ignored*/ ) {
		return { container, MessagePackBase::DefaultKeyExtractor<Container>(), MessagePackBase::DefaultValueExtractor<Container>() };
	}

// ---------------------------------------------------

	template <typename... Fields>
	ETInlineHint bool MessagePackWriter::operator()( Fields&&... fields ) {
		static_assert( 0u != sizeof...(fields), "MessagePackWriter::operator() must be called with at least one field!" );
		return Write( ::std::forward<Fields>( fields )... );
	}

// ---------------------------------------------------

	template <typename Head, typename... Tail>
	bool MessagePackWriter::Write( Head&& head, Tail&&... tail ) {
		return Write( ::std::forward<Head>( head ) ) ? Write( ::std::forward<Tail>( tail )... ) : false;
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<Utility::MessagePackWriter::Nil>( Utility::MessagePackWriter::Nil&& /*value*/ ) {
		return ::cmp_write_nil( this );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<bool>( bool&& value ) {
		return ::cmp_write_bool( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::uint8>( ::Eldritch2::uint8&& value ) {
		return ::cmp_write_u8( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::int8>( ::Eldritch2::int8&& value ) {
		return ::cmp_write_s8( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::uint16>( ::Eldritch2::uint16&& value ) {
		return ::cmp_write_u16( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::int16>( ::Eldritch2::int16&& value ) {
		return ::cmp_write_s16( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::uint32>( ::Eldritch2::uint32&& value ) {
		return ::cmp_write_u32( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::int32>( ::Eldritch2::int32&& value ) {
		return ::cmp_write_s32( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::uint64>( ::Eldritch2::uint64&& value ) {
		return ::cmp_write_u64( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::int64>( ::Eldritch2::int64&& value ) {
		return ::cmp_write_s64( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::float32>( ::Eldritch2::float32&& value ) {
		return ::cmp_write_float( this, value );
	}

// ---------------------------------------------------

	template <>
	bool MessagePackWriter::Write<::Eldritch2::float64>( ::Eldritch2::float64&& value ) {
		return ::cmp_write_double( this, value );
	}

// ---------------------------------------------------

	template <typename Head>
	bool MessagePackWriter::Write( Head&& head ) {
		return head.Serialize( *this );
	}

}	// namespace Utility
}	// namespace Eldritch2