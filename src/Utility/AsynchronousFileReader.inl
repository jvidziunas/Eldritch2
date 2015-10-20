/*==================================================================*\
  AsynchronousFileReader.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	ETInlineHint AsynchronousFileReader::BlockingResult::BlockingResult( ::Eldritch2::ErrorCode	readResult, size_t successfullyReadAmountInBytes ) : result( readResult ), readAmountInBytes( successfullyReadAmountInBytes ) {}

// ---------------------------------------------------

	ETForceInlineHint AsynchronousFileReader::BlockingResult::operator bool() const {
		return result;
	}

// ---------------------------------------------------

	ETForceInlineHint const void* AsynchronousFileReader::FixupReadPointer( const void* const rawReadPointer, const ::Eldritch2::uint64 desiredFileOffsetInBytes ) {
		return static_cast<const char*>(rawReadPointer) + (desiredFileOffsetInBytes % this->GetMediaSectorSizeInBytes());
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::uint64 AsynchronousFileReader::GetRoundedSizeInBytes() const {
		return this->GetRoundedReadSizeInBytes( this->GetSizeInBytes() );
	}

// ---------------------------------------------------

	ETInlineHint size_t AsynchronousFileReader::GetRoundedReadSizeInBytes( const size_t logicalReadSizeInBytes ) const {
		return ( logicalReadSizeInBytes - 1u ) / this->GetMediaSectorSizeInBytes() + 1u;
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::uint64 AsynchronousFileReader::GetOffsetForRead( const ::Eldritch2::uint64 logicalOffsetInBytes ) const {
		return logicalOffsetInBytes / this->GetMediaSectorSizeInBytes();
	}

}	// namespace FileSystem
}	// namespace Eldritch2