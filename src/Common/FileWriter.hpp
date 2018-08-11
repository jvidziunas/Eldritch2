/*==================================================================*\
  FileWriter.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class ErrorCode;
}

#if ET_PLATFORM_WINDOWS
using HANDLE = void*;
#endif

namespace Eldritch2 {

class FileWriter {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//! Disable copy construction.
	FileWriter(const FileWriter&) = delete;
	//!	Constructs this @ref FileWriter instance.
	FileWriter(FileWriter&&);
	//!	Constructs this @ref FileWriter instance.
	FileWriter();

	~FileWriter();

	// ---------------------------------------------------

public:
	//!	Performs a blocking write operation to the output device the @ref FileWriter uses as its backing.
	/*!	@param[in] sourceBuffer Pointer to a region of memory containing the bit patterns to be written.
		@param[in] lengthToWriteInBytes Number of bytes to write.
		@param[in] fileOffsetInBytes Offset into the file to begin writing data. This must be a multiple of the backing media sector size.
		@returns an @ref ErrorCode describing the result of the operation. */
	ErrorCode Write(const void* const sourceBuffer, size_t lengthToWriteInBytes, uint64 fileOffsetInBytes);
	//!	Performs a blocking write operation to the output device the @ref FileWriter uses as its backing.
	/*!	@param[in] sourceBuffer Pointer to a region of memory containing the bit patterns to be written.
		@param[in] lengthToWriteInBytes Number of bytes to write. This must be a multiple of the backing media sector size.
		@returns an @ref ErrorCode describing the result of the operation.
		@see @ref Write( const void*, const size_t, const uint64 ) */
	ErrorCode Append(const void* const sourceBuffer, size_t lengthToWriteInBytes);

	// ---------------------------------------------------

public:
	//! Returns the current implicit write offset, specified relative to the beginning of the file.
	/*!	@returns The offset, in bytes. */
	uint64 GetFileCursorInBytes() const;

	//!	Moves the internal file write pointer to immediately past the end of all written data.
	void AdvanceToEnd();

	//!	Extends or truncates the backing file so it exactly occupies the specified amount of space on the backing storage.
	/*!	@param[in] sizeInBytes The amount of valid data to mark the backing file as possessing. Note that this does _not_ need to be a multiple of the media sector size. */
	void SetSize(uint64 sizeInBytes);

	// ---------------------------------------------------

public:
	ErrorCode CreateOrTruncate(const PlatformChar* path);

	ErrorCode CreateOrOpen(const PlatformChar* path);

	ErrorCode Open(const PlatformChar* path);

	// ---------------------------------------------------

	//!	Disable copy assignment.
	FileWriter& operator=(const FileWriter&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	HANDLE _file;
#else
	static_assert(false, "FileWriter needs implementation for your platform!");
#endif
};

} // namespace Eldritch2
