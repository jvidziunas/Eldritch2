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
#include <Common/Containers/AbstractStringSpan.hpp>
#include <Common/Memory/EaStlAllocatorMixin.hpp>
#include <Common/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <eastl/string.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Character, class Allocator = MallocAllocator>
class AbstractString {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingContainer = eastl::basic_string<Character, EaStlAllocatorMixin<Allocator>>;

public:
	using Iterator             = typename UnderlyingContainer::iterator;
	using ConstIterator        = typename UnderlyingContainer::const_iterator;
	using ReverseIterator      = typename UnderlyingContainer::reverse_iterator;
	using ConstReverseIterator = typename UnderlyingContainer::const_reverse_iterator;
	using SizeType             = typename UnderlyingContainer::size_type;
	using CharacterType        = typename UnderlyingContainer::value_type;
	using ValueType            = typename UnderlyingContainer::value_type;
	using AllocatorType        = typename UnderlyingContainer::allocator_type::PublicType;
	using SliceType            = AbstractStringSpan<CharacterType>;
	using ConstSliceType       = AbstractStringSpan<CharacterType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractString instance.
	template <typename... Arguments>
	AbstractString(const AllocatorType& allocator, AbstractStringSpan<CharacterType> format, Arguments&&... arguments);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AllocatorType& allocator = AllocatorType(), SizeType reservedLength = 0u);
	//!	Constructs this @ref AbstractString instance.
	template <typename InputIterator>
	AbstractString(const AllocatorType& allocator, InputIterator begin, InputIterator end);
	//!	Constructs this @ref AbstractString instance.
	template <typename Character2>
	AbstractString(const AllocatorType& allocator, AbstractStringSpan<Character2> string);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AllocatorType& allocator, const AbstractString&);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AbstractString&) = default;
	//!	Constructs this @ref AbstractString instance.
	AbstractString(AbstractString&&) ETNoexceptHint = default;

	~AbstractString() = default;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	operator AbstractStringSpan<CharacterType>() const ETNoexceptHint;

	operator const CharacterType*() const ETNoexceptHint;

	SliceType Slice(SizeType offset, SizeType length) const ETNoexceptHint;

	const CharacterType* AsCString() const ETNoexceptHint;

	const CharacterType* GetData() const ETNoexceptHint;
	CharacterType*       GetData() ETNoexceptHint;

	// - ALGORITHMS --------------------------------------

public:
	SizeType Find(AbstractStringSpan<CharacterType> needle, SizeType offset = 0u) const ETNoexceptHint;
	SizeType Find(CharacterType character, SizeType offset = 0u) const ETNoexceptHint;

	SizeType FindLast(AbstractStringSpan<CharacterType> needle, SizeType offset = 0u) const ETNoexceptHint;
	SizeType FindLast(CharacterType character, SizeType offset = 0u) const ETNoexceptHint;

	bool Contains(AbstractStringSpan<CharacterType> string) const ETNoexceptHint;
	bool Contains(CharacterType character) const ETNoexceptHint;

	bool StartsWith(AbstractStringSpan<CharacterType> prefix) const ETNoexceptHint;
	bool StartsWith(CharacterType prefix) const ETNoexceptHint;

	bool EndsWith(AbstractStringSpan<CharacterType> suffix) const ETNoexceptHint;
	bool EndsWith(CharacterType suffix) const ETNoexceptHint;

	// ---------------------------------------------------

public:
	void MakeLowerCase();

	void MakeUpperCase();

	void ReplaceAll(CharacterType value, CharacterType replacement);

	// ---------------------------------------------------

public:
	ConstIterator Erase(ConstIterator begin, ConstIterator end);
	ConstIterator Erase(ConstIterator where);

	void Trim(SizeType charactersToAdvance, SizeType charactersToRemove);
	void Trim(ConstIterator newBegin, ConstIterator newEnd);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator Begin() const ETNoexceptHint;

	ConstIterator End() const ETNoexceptHint;

	ConstReverseIterator ReverseBegin() const ETNoexceptHint;

	ConstReverseIterator ReverseEnd() const ETNoexceptHint;

	ConstIterator ConstBegin() const ETNoexceptHint;

	ConstIterator ConstEnd() const ETNoexceptHint;

	ConstReverseIterator ConstReverseBegin() const ETNoexceptHint;

	ConstReverseIterator ConstReverseEnd() const ETNoexceptHint;

	CharacterType Front() const;

	CharacterType Back() const;

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <typename Character2>
	AbstractString& Assign(AbstractStringSpan<Character2> string);
	template <typename... Arguments>
	AbstractString& Assign(AbstractStringSpan<CharacterType> format, Arguments&&... arguments);
	AbstractString& Assign(AbstractStringSpan<CharacterType> string);

	AbstractString& operator=(AbstractStringSpan<CharacterType>);
	template <typename Character2>
	AbstractString& operator=(AbstractStringSpan<Character2>);
	AbstractString& operator=(const AbstractString&) = default;
	AbstractString& operator=(AbstractString&&) = default;

	// - CONTAINER MANIPULATION --------------------------

public:
	AbstractString& EnsurePrefix(AbstractStringSpan<CharacterType> prefix);
	AbstractString& EnsurePrefix(CharacterType prefix);

	AbstractString& EnsureSuffix(AbstractStringSpan<CharacterType> suffix);
	AbstractString& EnsureSuffix(CharacterType suffix);

	AbstractString& Prepend(AbstractStringSpan<CharacterType> string);
	AbstractString& Prepend(CharacterType value);

	template <typename... Arguments>
	AbstractString& Append(AbstractStringSpan<CharacterType> format, Arguments&&... arguments);
	AbstractString& Append(AbstractStringSpan<CharacterType> string);
	template <typename Character2>
	AbstractString& Append(AbstractStringSpan<Character2> string);
	template <typename InputIterator>
	AbstractString& Append(InputIterator begin, InputIterator end);
	AbstractString& Append(CharacterType value);

	AbstractString& operator+=(AbstractStringSpan<CharacterType> string);
	template <typename Character2>
	AbstractString& operator+=(AbstractStringSpan<Character2> string);
	AbstractString& operator+=(CharacterType value);

	// ---------------------------------------------------

public:
	void ForceSize(SizeType size);

	void Resize(SizeType size);

	void Clear() ETNoexceptHint;

	// - CONTAINER COMPARISON ----------------------------

public:
	int CompareCaseInsensitive(AbstractStringSpan<CharacterType> string) const ETNoexceptHint;

	int Compare(AbstractStringSpan<CharacterType> string) const ETNoexceptHint;

	// - ELEMENT ACCESS ----------------------------------

public:
	CharacterType operator[](SizeType offset) const;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetLength() const ETNoexceptHint;

	bool IsEmpty() const ETNoexceptHint;

	explicit operator bool() const ETNoexceptHint;

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacityInCharacters() const ETNoexceptHint;

	void Reserve(SizeType capacityInCharacters);

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingContainer _container;

	// ---------------------------------------------------

	template <typename Character, class Allocator>
	friend void Swap(AbstractString<Character, Allocator>&, AbstractString<Character, Allocator>&) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/AbstractString.inl>
//------------------------------------------------------------------//
