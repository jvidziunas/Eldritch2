/*==================================================================*\
  MessagePackBase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MessagePackReader.hpp>
#include <Utility/MessagePackWriter.hpp>
#include <Utility/MessagePackBase.hpp>
//------------------------------------------------------------------//

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
//	Disable warnings about type conversion/promotion.
#	pragma warning( disable : 4800 )
#	pragma warning( disable : 4244 )
#endif
extern "C" {
#include <cmp/cmp.c>
}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

	bool MessagePackBase::Nil::Serialize( MessagePackReader& reader ) {
		return ::cmp_read_nil( &reader );
	}

// ---------------------------------------------------

	bool MessagePackBase::Nil::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_nil( &writer );
	}

// ---------------------------------------------------

	MessagePackBase::String::String( const UTF8Char* begin, const UTF8Char* const end ) : Range<const UTF8Char*>( begin, end ) {}

// ---------------------------------------------------

	MessagePackBase::String::String() : Range<const UTF8Char*>( Range<const UTF8Char*>::EmptySet() ) {}

// ---------------------------------------------------

	bool MessagePackBase::String::Serialize( MessagePackReader& reader ) {
		uint32 lengthInOctets;

		if( ::cmp_read_str_size( &reader, &lengthInOctets ) ) {
			// const UTF8Char*	stringBegin( reader.SkipBytes() );
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	bool MessagePackBase::String::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_str( &writer, first, static_cast<uint32>(Size()) );
	}

// ---------------------------------------------------

	MessagePackBase::BinaryData::BinaryData( const void* begin, const void* const end ) : Range<const char*>( static_cast<const char*>(begin), static_cast<const char*>(end) ) {}

// ---------------------------------------------------

	MessagePackBase::BinaryData::BinaryData() : Range<const char*>( Range<const char*>::EmptySet() ) {}

// ---------------------------------------------------

	bool MessagePackBase::BinaryData::Serialize( MessagePackReader& reader ) {
		uint32 lengthInBytes;

		if( ::cmp_read_bin_size( &reader, &lengthInBytes ) ) {
			// const UTF8Char*	stringBegin( reader.SkipBytes() );
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	bool MessagePackBase::BinaryData::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_bin( &writer, first, static_cast<uint32>(Size()) );
	}

// ---------------------------------------------------

	MessagePackBase::Array::Array( const uint32 elementCount ) : sizeInElements( elementCount ) {}

// ---------------------------------------------------

	bool MessagePackBase::Array::Serialize( MessagePackReader& reader ) {
		return ::cmp_read_array( &reader, &sizeInElements );
	}

// ---------------------------------------------------

	bool MessagePackBase::Array::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_array( &writer, sizeInElements );
	}

// ---------------------------------------------------

	MessagePackBase::Map::Map( const uint32 pairCount ) : sizeInPairs( pairCount ) {}

// ---------------------------------------------------

	bool MessagePackBase::Map::Serialize( MessagePackReader& reader ) {
		return ::cmp_read_array( &reader, &sizeInPairs );
	}

// ---------------------------------------------------

	bool MessagePackBase::Map::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_array( &writer, sizeInPairs );
	}

// ---------------------------------------------------

	MessagePackBase::MessagePackBase( const ::cmp_reader readFunction, const void* const buffer ) {
		::cmp_init( this, const_cast<void*>(buffer), readFunction, [] ( cmp_ctx_t* /*context*/, const void* /*sourceData*/, size_t /*sourceLengthInBytes*/ ) -> size_t { return 0u; } );
	}

// ---------------------------------------------------

	MessagePackBase::MessagePackBase( const ::cmp_writer writeFunction, void* const buffer ) {
		::cmp_init( this, buffer, [] ( ::cmp_ctx_t* /*self*/, void* /*destinationBuffer*/, size_t /*requestedSizeInBytes*/ ) { return false; }, writeFunction );
	}

}	// namespace Utility
}	// namespace Eldritch2
