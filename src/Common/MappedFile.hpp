/*==================================================================*\
  MappedFile.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
class ErrorCode;
}

namespace Eldritch2 {

class MappedFile {
	// - TYPE PUBLISHING ---------------------------------

public:
	enum AccessType {
		Read  = 0,
		Write = 1,
		All   = (Read | Write)
	};

	// ---

public:
	enum : size_t {
		LengthOfFile = ~size_t(0)
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Disable copy construction.
	MappedFile(const MappedFile&) = delete;
	//!	Constructs this @ref MappedFile instance.
	MappedFile(MappedFile&&) ETNoexceptHint;
	//!	Constructs this @ref MappedFile instance.
	MappedFile() ETNoexceptHint;

	~MappedFile();

	// ---------------------------------------------------

public:
	ErrorCode ClearOrCreate(AccessType access, const PlatformChar* path, size_t byteSize) ETNoexceptHint;

	ErrorCode Open(AccessType access, const PlatformChar* path, uint64 byteOffset = 0u, size_t byteSize = LengthOfFile) ETNoexceptHint;

	// ---------------------------------------------------

public:
	template <typename Struct>
	Range<Struct*> GetRange(size_t byteOffset, size_t lengthInElements) const ETNoexceptHint;

	template <typename Struct>
	Struct* Get(size_t byteOffset) const ETNoexceptHint;

	void* Get(size_t byteOffset) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	size_t GetSizeInBytes() const ETNoexceptHint;

	bool HasAccess(AccessType access) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	void Prefetch(size_t byteOffset, size_t byteLength) const ETNoexceptHint;
	void Prefetch(Range<const char*> range) const ETNoexceptHint;

	void Evict(size_t byteOffset, size_t byteLength) const ETNoexceptHint;
	void Evict(Range<const char*> range) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	//!	Disable copy assignment.
	MappedFile& operator=(const MappedFile&) = delete;

	// - DATA MEMBERS ------------------------------------

private:
#if ET_PLATFORM_WINDOWS
	AccessType   _access;
	Range<char*> _mapping;
#else
	static_assert(false, "MappedFile needs implementation for target platform!");
#endif // if ET_PLATFORM_WINDOWS

	// ---------------------------------------------------

	friend void Swap(MappedFile&, MappedFile&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/MappedFile.inl>
//------------------------------------------------------------------//
