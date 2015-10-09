/*==================================================================*\
  SynchronousFileWriter.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	class ETPureAbstractHint SynchronousFileWriter {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct BlockingResult {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this BlockingResult instance.
			/*!	@param[in] writeResult a ResultCode describing the ultimate result of the write operation, optionally containing information on why a failure occurred.
				@param[in] successfullyWrittenAmountInBytes Amount of source data successfully committed to backing storage before a failure occurred.
				*/
			ETInlineHint BlockingResult( const ::Eldritch2::ErrorCode writeResult, const size_t successfullyWrittenAmountInBytes );
			//!	Constructs this BlockingResult instance.
			/*!	@param[in] itemTemplate the template BlockingResult that will be copied.
				*/
			ETInlineHint BlockingResult( const BlockingResult& itemTemplate ) = default;

			ETInlineHint operator bool() const;

		// - DATA MEMBERS ------------------------------------

			//!	The final state of the write operation.
			::Eldritch2::ErrorCode	result;

			//! Amount of data successfully written before a failure case, or the original size of the write request in the event no errors were encountered.
			size_t					writtenAmountInBytes;
		};

	// ---------------------------------------------------

		//!	Performs a blocking write operation to the output device this SynchronousFileWriter uses as its backing.
		/*!	@param[in] sourceBuffer Pointer to a region of memory containing the bit patterns to be written.
			@param[in] lengthToWriteInBytes Number of bytes to write.
			@param[in] fileOffsetInBytes Offset into the file to begin writing data. This must be a multiple of the backing media sector size.
			@returns a BlockingResult describing the result of the operation.
			@sa GetPhysicalMediaSectorSizeInBytes()
			*/
		virtual BlockingResult	Write( const void* sourceBuffer, const size_t lengthToWriteInBytes, const ::Eldritch2::uint64 fileOffsetInBytes ) abstract;
		//!	Performs a blocking write operation to the output device this SynchronousFileWriter uses as its backing.
		/*!	@param[in] sourceBuffer Pointer to a region of memory containing the bit patterns to be written.
			@param[in] lengthToWriteInBytes Number of bytes to write. This must be a multiple of the backing media sector size.
			@returns a BlockingResult describing the result of the operation.
			@sa GetPhysicalMediaSectorSizeInBytes()
			@sa Write( const void*, const size_t, const ::Eldritch2::uint64 )
			*/
		virtual BlockingResult	Write( const void* sourceBuffer, const size_t lengthToWriteInBytes ) abstract;

	// ---------------------------------------------------

		//! Returns the current implicit write offset, specified relative to the beginning of the file.
		/*!	@returns The offset, in bytes.
			*/
		virtual ::Eldritch2::uint64	GetFileCursorInBytes() const abstract;

		//!	Moves the internal file write pointer to immediately past the end of all written data.
		virtual void				AdvanceToEnd() abstract;

		//!	Extends or truncates the backing file so it exactly occupies the specified amount of space on the backing storage.
		/*!	@param[in] fileSizeInBytes The amount of valid data to mark the backing file as possessing. Note that this does _not_ need to be a multiple of the media sector size.
			*/
		virtual void				SetFileSize( const ::Eldritch2::uint64 fileSizeInBytes ) abstract;

		//! Retrieves the physical media sector size for this file/pseudofile.
		/*!	@returns The sector size for the file.
			@remark All write offsets passed to Write(const void*, const size_t, const ::Eldritch2::uint64) must be multiples of this value.
			@remark All write lengths passed to Write() must be a multiple of this value.
			@remark Thread-safe.
			@sa Write()
			*/
		virtual size_t				GetPhysicalMediaSectorSizeInBytes() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this SynchronousFileWriter instance.
		SynchronousFileWriter( const SynchronousFileWriter& ) = delete;
		//! Constructs this SynchronousFileWriter instance.
		/*!	@remark This is designed to be called only from child classes.
			*/
		SynchronousFileWriter() = default;

	public:
		//! Destroys this SynchronousFileWriter instance.
		virtual ~SynchronousFileWriter() = default;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/SynchronousFileWriter.inl>
//------------------------------------------------------------------//