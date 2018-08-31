/*==================================================================*\
  Path.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
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
	ImageDirectory,
	Logs
};

// ---

template <class Allocator = MallocAllocator>
class AbstractPath : public AbstractString<PlatformChar, Allocator> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractPath instance.
	template <typename... Arguments>
	AbstractPath(const AllocatorType& allocator, KnownDirectory root, AbstractStringView<CharacterType> format, Arguments&&... arguments);
	//!	Constructs this @ref AbstractPath instance.
	template <typename Character>
	AbstractPath(const AllocatorType& allocator, KnownDirectory root, AbstractStringView<Character> path);
	//!	Constructs this @ref AbstractPath instance.
	AbstractPath(const AllocatorType& allocator, AbstractStringView<CharacterType> path);
	//!	Constructs this @ref AbstractPath instance.
	AbstractPath(const AllocatorType& allocator, SizeType reservedLength);
	//!	Constructs this @ref AbstractPath instance.
	AbstractPath(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref AbstractPath instance.
	AbstractPath(const AbstractPath&) = default;
	//!	Constructs this @ref AbstractPath instance.
	AbstractPath(AbstractPath&&) = default;

	~AbstractPath() = default;

	// ---------------------------------------------------

public:
	template <typename... Arguments>
	AbstractPath& Assign(KnownDirectory root, AbstractStringView<CharacterType> format, Arguments&&... arguments);
	template <typename Character>
	AbstractPath& Assign(KnownDirectory root, AbstractStringView<Character> path);
	using AbstractString<PlatformChar, Allocator>::Assign;

	// ---------------------------------------------------

	template <class Allocator>
	friend void Swap(AbstractPath<Allocator>&, AbstractPath<Allocator>&);
};

// ---

AbstractStringView<PlatformChar> GetPath(KnownDirectory directory);

using Path = AbstractPath<MallocAllocator>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Path.inl>
//------------------------------------------------------------------//
