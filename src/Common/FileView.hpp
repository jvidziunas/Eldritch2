/*==================================================================*\
  FileView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Span.hpp>
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
enum class Result : int;
}

namespace Eldritch2 {

class FileView {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum FileAccess {
		Read  = 0,
		Write = 1,
		All   = (Read | Write)
	};

	// ---

public:
	enum : size_t {
		BeginningOfFile = 0u,
		EndOfFile       = ~size_t(0u)
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	FileView(const FileView&) = delete;
	//!	Constructs this @ref MappedFile instance.
	FileView(FileView&&) ETNoexceptHint;
	//!	Constructs this @ref MappedFile instance.
	FileView() ETNoexceptHint;

	~FileView();

	// ---------------------------------------------------

public:
	Result ClearOrCreate(FileAccess access, const PlatformChar* path, size_t byteSize) ETNoexceptHint;

	Result Open(FileAccess access, const PlatformChar* path, uint64 byteOffset = BeginningOfFile, size_t byteSize = EndOfFile) ETNoexceptHint;

	// ---------------------------------------------------

public:
	Span<const byte*> Slice(size_t byteOffset, size_t byteLength) const ETNoexceptHint;
	template <typename Struct>
	Span<Struct*> Slice(size_t byteOffset, size_t lengthInElements) const ETNoexceptHint;

	template <typename Character>
	AbstractStringSpan<Character> StringSlice(size_t byteOffset, typename AbstractStringSpan<Character>::SizeType length) const ETNoexceptHint;

	template <typename Struct>
	Struct* Get(size_t byteOffset) const ETNoexceptHint;

	void* Get(size_t byteOffset) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	FileTime GetLastWriteTime() const ETNoexceptHint;

	size_t GetByteSize() const ETNoexceptHint;

	bool HasAccess(FileAccess access) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	void Prefetch(size_t byteOffset, size_t byteLength) const ETNoexceptHint;
	void Prefetch(Span<const byte*> range) const ETNoexceptHint;

	void Evict(size_t byteOffset, size_t byteLength) const ETNoexceptHint;
	void Evict(Span<const byte*> range) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	//!	Disable copy assignment.
	FileView& operator=(const FileView&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	FileAccess   _access;
	HANDLE       _file;
	Span<byte*> _view;
#else
	static_assert(false, "MappedFile needs implementation for target platform!");
#endif // if ET_PLATFORM_WINDOWS

	// ---------------------------------------------------

	friend void Swap(FileView&, FileView&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/FileView.inl>
//------------------------------------------------------------------//
