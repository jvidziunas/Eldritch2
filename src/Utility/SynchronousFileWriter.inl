/*==================================================================*\
  SynchronousFileWriter.inl
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

	ETInlineHint SynchronousFileWriter::BlockingResult::BlockingResult( const ::Eldritch2::ErrorCode writeResult, const size_t successfullyWrittenAmountInBytes ) : result( writeResult ),  writtenAmountInBytes( successfullyWrittenAmountInBytes ) {}

// ---------------------------------------------------

	ETInlineHint SynchronousFileWriter::BlockingResult::operator bool() const {
		return static_cast<bool>(result);
	}

}	// namespace FileSystem
}	// namespace Eldritch2