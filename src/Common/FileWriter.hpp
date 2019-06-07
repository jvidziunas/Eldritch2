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
enum class Result : int;
} // namespace Eldritch2

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
	FileWriter(FileWriter&&) ETNoexceptHint;
	//!	Constructs this @ref FileWriter instance.
	FileWriter() ETNoexceptHint;

	~FileWriter();

	// ---------------------------------------------------

public:
	//!	Performs a blocking write operation to the output device the @ref FileWriter uses as its backing.
	/*!	@param[in] source Pointer to a region of memory containing the bit patterns to be written.
		@param[in] byteLength Number of bytes to write.
		@param[in] fileByteOffset Offset into the file to begin writing data. This must be a multiple of the backing media sector size.
		@returns an @ref ErrorCode describing the result of the operation. */
	Result Write(const void* source, size_t byteLength, uint64 fileByteOffset) ETNoexceptHint;
	//!	Performs a blocking write operation to the output device the @ref FileWriter uses as its backing.
	/*!	@param[in] source Pointer to a region of memory containing the bit patterns to be written.
		@param[in] byteLength Number of bytes to write. This must be a multiple of the backing media sector size.
		@returns an @ref ErrorCode describing the result of the operation.
		@see @ref Write(const void*, size_t, uint64) */
	Result Append(const void* source, size_t byteLength) ETNoexceptHint;

	// ---------------------------------------------------

public:
	//! Returns the current implicit write offset, specified relative to the beginning of the file.
	/*!	@returns The offset, in bytes. */
	uint64 GetFileCursorInBytes() const ETNoexceptHint;

	//!	Moves the internal file write pointer to immediately past the end of all written data.
	void AdvanceToEnd() ETNoexceptHint;

	//!	Extends or truncates the backing file so it exactly occupies the specified amount of space on the backing storage.
	/*!	@param[in] sizeInBytes The amount of valid data to mark the backing file as possessing. Note that this does _not_ need to be a multiple of the media sector size. */
	void SetSize(uint64 sizeInBytes) ETNoexceptHint;

	// ---------------------------------------------------

public:
	Result CreateOrTruncate(const PlatformChar* path) ETNoexceptHint;

	Result CreateOrOpen(const PlatformChar* path) ETNoexceptHint;

	Result Open(const PlatformChar* path) ETNoexceptHint;

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
