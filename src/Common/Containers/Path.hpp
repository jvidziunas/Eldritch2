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

enum class KnownDirectory : unsigned int {
	Relative,
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
	// - TYPE PUBLISHING ---------------------------------

public:
	using Iterator             = typename AbstractString<PlatformChar, Allocator>::Iterator;
	using ConstIterator        = typename AbstractString<PlatformChar, Allocator>::ConstIterator;
	using ReverseIterator      = typename AbstractString<PlatformChar, Allocator>::ReverseIterator;
	using ConstReverseIterator = typename AbstractString<PlatformChar, Allocator>::ConstReverseIterator;
	using SizeType             = typename AbstractString<PlatformChar, Allocator>::SizeType;
	using CharacterType        = typename AbstractString<PlatformChar, Allocator>::CharacterType;
	using ValueType            = typename AbstractString<PlatformChar, Allocator>::ValueType;
	using AllocatorType        = typename AbstractString<PlatformChar, Allocator>::AllocatorType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractPath instance.
	template <typename... Arguments>
	AbstractPath(const AllocatorType& allocator, KnownDirectory root, AbstractStringSpan<CharacterType> format, Arguments&&... arguments);
	//!	Constructs this @ref AbstractPath instance.
	template <typename Character>
	AbstractPath(const AllocatorType& allocator, KnownDirectory root, AbstractStringSpan<Character> path);
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
	AbstractPath& Assign(KnownDirectory root, AbstractStringSpan<CharacterType> format, Arguments&&... arguments);
	template <typename Character>
	AbstractPath& Assign(KnownDirectory root, AbstractStringSpan<Character> path);
	using AbstractString<PlatformChar, Allocator>::Assign;

	// ---------------------------------------------------

	template <class Allocator>
	friend void Swap(AbstractPath<Allocator>&, AbstractPath<Allocator>&) ETNoexceptHint;
};

// ---

AbstractStringSpan<PlatformChar> GetPath(KnownDirectory directory);

using Path = AbstractPath<MallocAllocator>;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Path.inl>
//------------------------------------------------------------------//
