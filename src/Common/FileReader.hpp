/*==================================================================*\
  FileReader.hpp
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

class FileReader {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	FileReader(const FileReader&) = delete;
	//!	Constructs this @ref FileReader instance.
	FileReader(FileReader&&);
	//!	Constructs this @ref FileReader instance.
	FileReader();

	~FileReader();

	// ---------------------------------------------------

public:
	//! Performs a blocking read operation to the output device this @ref FileReader uses as its backing.
	ErrorCode Read(void* const destination, size_t lengthToReadInBytes, uint64 fileOffsetInBytes);
	//! Performs a blocking read operation to the output device this @ref FileReader uses as its backing.
	ErrorCode Read(void* const destination, size_t lengthToReadInBytes);

	// ---------------------------------------------------

public:
	//!	Retrieves the total on-disk size of the file/pseudofile being accessed.
	/*!	@returns On disk size of the file, in bytes.
		@remarks Thread-safe. */
	uint64 GetSizeInBytes() const;

	// ---------------------------------------------------

public:
	ErrorCode Open(const PlatformChar* path);

	// ---------------------------------------------------

	//!	Disable copy assignment.
	FileReader& operator=(const FileReader&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	HANDLE _file;
#else
	static_assert(false, "FileReader needs implementation for target platform!");
#endif
};

} // namespace Eldritch2
