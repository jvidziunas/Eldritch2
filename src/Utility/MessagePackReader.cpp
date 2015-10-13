/*==================================================================*\
  MessagePackReader.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/MessagePackReader.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

	MessagePackReader::MessagePackReader( const Range<const char*> encodedData ) : MessagePackBase( [] ( ::cmp_ctx_t* self, void* destinationBuffer, size_t requestedSizeInBytes ) { return static_cast<MessagePackReader*>(self)->ReadRaw( destinationBuffer, requestedSizeInBytes ); }, encodedData.first ),
																				   _encodedDataEnd( encodedData.onePastLast ) {}

// ---------------------------------------------------

	bool MessagePackReader::ReadRaw( void* const destination, const size_t readSizeInBytes ) {
		const char* const	sourceData( static_cast<char*>(buf) );

		if( static_cast<size_t>(_encodedDataEnd - sourceData) <= readSizeInBytes ) {
			CopyMemory( destination, sourceData, readSizeInBytes );
			buf = const_cast<char*>(sourceData) + readSizeInBytes;
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	bool MessagePackReader::SkipBytes( const size_t skipSpanInBytes ) {
		const char* const	readPointer( static_cast<char*>(buf) );

		if( readPointer && (readPointer + skipSpanInBytes) < _encodedDataEnd ) {
			buf = static_cast<char*>(buf) + skipSpanInBytes;
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	bool MessagePackReader::GetInPlaceDataSpan( const size_t spanSizeInBytes, Range<const char*>& range ) {
		const char* const	readPointer( static_cast<char*>(buf) );

		if( SkipBytes( spanSizeInBytes ) ) {
			range = { readPointer, readPointer + spanSizeInBytes };
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	MessagePackReader::Checkpoint MessagePackReader::CreateCheckpoint() const {
		return buf;
	}

// ---------------------------------------------------

	void MessagePackReader::RestoreCheckpoint( const Checkpoint checkpoint ) {
		// Verify the checkpoint is legal.
		ETRuntimeAssert( checkpoint <= buf );

		buf = const_cast<void*>(checkpoint);
	}

}	// namespace Utility
}	// namespace Eldritch2