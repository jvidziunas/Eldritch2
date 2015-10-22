/*==================================================================*\
  AsynchronousFileWriter.inl
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

	ETInlineHint AsynchronousFileWriter::BlockingResult::BlockingResult( const BlockingResult& itemTemplate ) : result( itemTemplate.result ), writtenAmountInBytes( itemTemplate.writtenAmountInBytes ) {}

// ---------------------------------------------------

	ETInlineHint AsynchronousFileWriter::BlockingResult::BlockingResult( ::Eldritch2::ErrorCode	writeResult, size_t successfullyWrittenAmountInBytes ) : result( writeResult ), writtenAmountInBytes( successfullyWrittenAmountInBytes ) {}

}	// namespace FileSystem
}	// namespace Eldritch2