/*==================================================================*\
  FileAppender.hpp
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

class FileAppender {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	FileAppender(const FileAppender&) = delete;
	//!	Constructs this @ref FileAppender instance.
	FileAppender(FileAppender&&);
	//!	Constructs this @ref FileAppender instance.
	FileAppender();

	~FileAppender();

	// ---------------------------------------------------

public:
	ErrorCode CreateOrTruncate(const PlatformChar* path);

	ErrorCode OpenOrCreate(const PlatformChar* path);

	ErrorCode Open(const PlatformChar* path);

	// ---------------------------------------------------

public:
	ErrorCode Append(const void* const sourceData, size_t writeLengthInBytes);

	// ---------------------------------------------------

public:
	//!	Disable copy assignment.
	FileAppender& operator=(const FileAppender&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	HANDLE _file;
#else
	static_assert(false, "FileAppender needs implementation for target platform!");
#endif

	// ---------------------------------------------------

	friend void Swap(FileAppender&, FileAppender&);
};

} // namespace Eldritch2
