/*==================================================================*\
  AbstractString.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Containers/StringView.hpp>
//------------------------------------------------------------------//
#include <eastl/string.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

struct ReturnEndOfNeedleSemantics {};

template <typename Character, class Allocator = MallocAllocator>
class AbstractString {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::basic_string<Character, EaStlAllocatorMixin<Allocator>>;

public:
	using Iterator      = typename UnderlyingContainer::iterator;
	using ConstIterator = typename UnderlyingContainer::const_iterator;
	using SizeType      = typename UnderlyingContainer::size_type;
	using CharacterType = typename UnderlyingContainer::value_type;
	using AllocatorType = typename UnderlyingContainer::allocator_type::PublicType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractString instance.
	template <typename AlternateCharacter>
	AbstractString(StringView<AlternateCharacter>, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref AbstractString instance.
	AbstractString(StringView<CharacterType>, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref AbstractString instance.
	AbstractString(SizeType reservedLength, const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AbstractString& string, const AllocatorType& allocator);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AllocatorType& allocator = AllocatorType());
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AbstractString&) = default;
	//!	Constructs this @ref AbstractString instance.
	AbstractString(AbstractString&&) = default;

	~AbstractString() = default;

	// - ALGORITHMS --------------------------------------

public:
	ConstIterator FindFirstInstance(ConstIterator start, StringView<CharacterType> needle, const ReturnEndOfNeedleSemantics) const;
	ConstIterator FindFirstInstance(ConstIterator start, StringView<CharacterType> needle) const;
	ConstIterator FindFirstInstance(StringView<CharacterType> needle, const ReturnEndOfNeedleSemantics) const;
	ConstIterator FindFirstInstance(StringView<CharacterType> needle) const;
	ConstIterator FindFirstInstance(ConstIterator start, CharacterType character) const;
	ConstIterator FindFirstInstance(CharacterType character) const;

	ConstIterator FindLastInstance(ConstIterator start, StringView<CharacterType> needle, const ReturnEndOfNeedleSemantics) const;
	ConstIterator FindLastInstance(ConstIterator start, StringView<CharacterType> needle) const;
	ConstIterator FindLastInstance(StringView<CharacterType> needle, const ReturnEndOfNeedleSemantics) const;
	ConstIterator FindLastInstance(StringView<CharacterType> needle) const;
	ConstIterator FindLastInstance(ConstIterator start, CharacterType character) const;
	ConstIterator FindLastInstance(CharacterType character) const;

	bool Contains(StringView<CharacterType> string) const;
	bool Contains(const CharacterType character) const;

	bool HasPrefix(StringView<CharacterType> string) const;
	bool HasPrefix(const CharacterType character) const;

	bool HasSuffix(StringView<CharacterType> string) const;
	bool HasSuffix(const CharacterType character) const;

	// ---------------------------------------------------

public:
	void MakeLowerCase();

	void MakeUpperCase();

	// ---------------------------------------------------

public:
	void Replace(CharacterType source, CharacterType replacement);

	// ---------------------------------------------------

public:
	ConstIterator Erase(ConstIterator begin, ConstIterator end);
	ConstIterator Erase(ConstIterator position);

	void Trim(SizeType charactersToAdvanceBeginning, SizeType charactersToRemoveFromEnd);
	void Trim(ConstIterator newBegin, ConstIterator newEnd);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator Begin() const;

	ConstIterator End() const;

	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <typename AlternateCharacter>
	AbstractString& Assign(StringView<AlternateCharacter> string);
	template <typename... Arguments>
	AbstractString& Assign(StringView<CharacterType> format, Arguments&&... arguments);
	AbstractString& Assign(StringView<CharacterType> string);

	template <typename AlternateCharacter>
	AbstractString& operator=(StringView<AlternateCharacter> string);
	AbstractString& operator=(StringView<CharacterType> string);
	AbstractString& operator=(AbstractString&&);

	// - CONTAINER MANIPULATION --------------------------

public:
	AbstractString& EnsureSuffix(StringView<CharacterType> string);
	AbstractString& EnsureSuffix(CharacterType character);

	template <typename AlternateCharacter>
	AbstractString& Append(StringView<AlternateCharacter> string);
	template <typename... Arguments>
	AbstractString& Append(StringView<CharacterType> format, Arguments&&... arguments);
	AbstractString& Append(StringView<CharacterType> string);
	AbstractString& Append(CharacterType character);

	template <typename AlternateCharacter>
	AbstractString& operator+=(StringView<AlternateCharacter> string);
	AbstractString& operator+=(StringView<CharacterType> string);
	AbstractString& operator+=(CharacterType character);

	// ---------------------------------------------------

public:
	template <typename... Arguments>
	static SizeType GetFormattedLength(StringView<CharacterType> format, Arguments&&... arguments);

	// ---------------------------------------------------

public:
	void Resize(SizeType size);

	void Clear();

	// - CONTAINER COMPARISON ----------------------------

public:
	int Compare(StringView<CharacterType> string) const;

	// - ELEMENT ACCESS ----------------------------------

public:
	operator StringView<CharacterType>() const;

	operator const CharacterType*() const;

	const CharacterType* AsCString() const;

	const CharacterType* GetData();

	CharacterType operator[](SizeType indexInCharacters) const;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetLength() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacityInCharacters() const;

	void Reserve(SizeType capacityInCharacters);

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Character, class Allocator>
	friend void Swap(AbstractString<Character, Allocator>&, AbstractString<Character, Allocator>&);
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/AbstractString.inl>
//------------------------------------------------------------------//
