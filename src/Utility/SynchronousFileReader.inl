/*==================================================================*\
  SynchronousFileReader.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	ETInlineHint SynchronousFileReader::BlockingResult::BlockingResult( const ::Eldritch2::ErrorCode readResult, const size_t successfullyReadAmountInBytes ) : result( readResult ), readAmountInBytes( successfullyReadAmountInBytes ) {}

// ---------------------------------------------------

	ETInlineHint uint64 SynchronousFileReader::GetRoundedSizeInBytes() const {
		return GetRoundedReadSizeInBytes( GetSizeInBytes() );
	}

// ---------------------------------------------------

	ETInlineHint size_t SynchronousFileReader::GetRoundedReadSizeInBytes( const size_t logicalReadSizeInBytes ) const {
		return ( logicalReadSizeInBytes - 1u ) / GetPhysicalMediaSectorSizeInBytes() + static_cast<uint64>( 1u );
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::uint64 SynchronousFileReader::GetOffsetForRead( const ::Eldritch2::uint64 logicalOffsetInBytes ) const {
		return logicalOffsetInBytes / GetPhysicalMediaSectorSizeInBytes();
	}

}	// namespace FileSystem
}	// namespace Eldritch2