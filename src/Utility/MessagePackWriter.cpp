/*==================================================================*\
  MessagePackWriter.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/SynchronousFileWriter.hpp>
#include <Utility/MessagePackWriter.hpp>
#include <Utility/Math/MathUtils.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

	MessagePackWriter::MessagePackWriter( SynchronousFileWriter& writer ) : MessagePackBase( [] ( ::cmp_ctx_t* context, const void* sourceData, size_t sourceLengthInBytes ) { return static_cast<MessagePackWriter*>(context)->WriteRaw( sourceData, sourceLengthInBytes ); } ),
																			_writer( writer ) {}

// ---------------------------------------------------

	size_t MessagePackWriter::WriteRaw( const void* const sourceData, const size_t sourceDataSizeInBytes ) {
		return _writer.Write( sourceData, sourceDataSizeInBytes ).writtenAmountInBytes;
	}

// ---------------------------------------------------

	uint64 MessagePackWriter::GetWriteCursorInBytes() const {
		return _writer.GetFileCursorInBytes();
	}

}	// namespace Utility
}	// namespace Eldritch2