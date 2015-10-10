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
// Disable warnings about type conversion/promotion.
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

	bool MessagePackBase::ArrayHeader::Serialize( MessagePackReader& reader ) {
		return ::cmp_read_array( &reader, &sizeInElements );
	}

// ---------------------------------------------------

	bool MessagePackBase::ArrayHeader::Serialize( MessagePackWriter& writer ) {
		return ::cmp_write_array( &writer, sizeInElements );
	}

// ---------------------------------------------------

	bool MessagePackBase::ObjectReference::Serialize( MessagePackReader& reader ) {
		return ::cmp_read_ext( &reader, )
	}

// ---------------------------------------------------

	bool MessagePackBase::ObjectReference::Serialize( MessagePackWriter& writer ) {

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
