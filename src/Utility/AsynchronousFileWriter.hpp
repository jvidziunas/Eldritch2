/*==================================================================*\
  AsynchronousFileWriter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint AsynchronousFileWriter : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct BlockingResult {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref BlockingResult instance.
			ETInlineHint BlockingResult( const BlockingResult& itemTemplate );
			//!	Constructs this @ref BlockingResult instance.
			ETInlineHint BlockingResult( ::Eldritch2::ErrorCode	writeResult, size_t successfullyWrittenAmountInBytes );

		// - DATA MEMBERS ------------------------------------

			//!	The final state of the write operation.
			::Eldritch2::ErrorCode	result;

			//!	Amount of data successfully written before a failure case, or the original size of the write request in the event no errors were encountered.
			size_t					writtenAmountInBytes;
		};

	// - ASYNCHRONOUS FILE WRITES-------------------------

		// Moves the internal file write pointer to the offset (relative to the beginning of the file) specified in fileOffsetInBytes, 
		// then begins an (optionally) asynchronous write of the data contained in the buffer identified by the sourceData parameter and spanning
		// the specified length. The buffer itself must persist at least until the next call to FinishWrite() returns.
		virtual ::Eldritch2::ErrorCode	BeginWrite( const void* const sourceBuffer, const size_t dataLengthInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) abstract;

		//! Determines whether or not a previous write operation initiated by a call to BeginWrite() has completed.
		virtual bool					IsWriteComplete() abstract;

		//! Blocks until the write begun by a previous call to BeginWrite() completes, returning the number of bytes successfully copied.
		virtual BlockingResult			BlockUntilWriteComplete() abstract;

		virtual void					CancelWrite() abstract;

	// - DISK GEOMETRY QUERIES ---------------------------

		//!	Retrieves the physical media sector size for this file/pseudofile. All source buffers must be minimally aligned to this value.
		// Thread-safe.
		virtual size_t	GetMediaSectorSizeInBytes() const abstract;

	// ---------------------------------------------------

		//!	Moves the internal file write pointer to immediately past the end of all written data.
		// Not thread-safe.
		virtual void	AdvanceToEnd() abstract;

		//!	Extends or truncates the on-disk data so the file is exactly the specified size from start to end.
		virtual void	SetFileSize( const ::Eldritch2::uint64 fileSizeInBytes ) abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref AsynchronousFileWriter instance.
		AsynchronousFileWriter() = default;

	public:
		//!	Destroys this @ref AsynchronousFileWriter instance.
		virtual ~AsynchronousFileWriter() = default;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/AsynchronousFileWriter.inl>
//------------------------------------------------------------------//