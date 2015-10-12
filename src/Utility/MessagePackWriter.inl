/*==================================================================*\
  MessagePackWriter.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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