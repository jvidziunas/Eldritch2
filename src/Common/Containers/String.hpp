/*==================================================================*\
  String.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractString.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

enum class KnownDirectory : uint32 {
	UserDocuments,
	AppDataLocal,
	AppDataShared,
	Packages,
	DownloadedPackages,
	WorkingDirectory,
	Logs
};

// ---

template <class Allocator = MallocAllocator>
using String = AbstractString<Utf8Char, Allocator>;

// ---

template <class Allocator = MallocAllocator>
class Path : public AbstractString<PlatformChar, Allocator> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Path instance.
	template <typename AlternateCharacter>
	Path(StringView<AlternateCharacter> string, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Path instance.
	Path(StringView<CharacterType> string, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Path instance.
	Path(SizeType reservedLength, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Path instance.
	Path(const Path& string, const AllocatorType& allocator);
	//!	Constructs this @ref Path instance.
	Path(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref Path instance.
	Path(const Path&) = default;
	//!	Constructs this @ref Path instance.
	Path(Path&&) = default;

	~Path() = default;

	// ---------------------------------------------------

public:
	template <typename... Arguments>
	Path& Assign(KnownDirectory root, StringView<CharacterType> pathFormat, Arguments&&... arguments);
	Path& Assign(KnownDirectory root, StringView<CharacterType> path);
	using AbstractString<PlatformChar, Allocator>::Assign;

	// ---------------------------------------------------

public:
	Path& StripDirectory();

	Path& StripFile();

	Path& StripExtension();
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/String.inl>
//------------------------------------------------------------------//
