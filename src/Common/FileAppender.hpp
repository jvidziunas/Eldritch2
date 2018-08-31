/*==================================================================*\
  FileAppender.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
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
	FileAppender(FileAppender&&) ETNoexceptHint;
	//!	Constructs this @ref FileAppender instance.
	FileAppender() ETNoexceptHint;

	~FileAppender();

	// ---------------------------------------------------

public:
	ErrorCode Append(const void* const sourceData, size_t writeLengthInBytes) ETNoexceptHint;

	// ---------------------------------------------------

public:
	ErrorCode CreateOrTruncate(const PlatformChar* path) ETNoexceptHint;

	ErrorCode OpenOrCreate(const PlatformChar* path) ETNoexceptHint;

	ErrorCode Open(const PlatformChar* path) ETNoexceptHint;

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

	friend void Swap(FileAppender&, FileAppender&) ETNoexceptHint;
};

} // namespace Eldritch2
