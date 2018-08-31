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
#include <Common/Containers/AbstractStringView.hpp>
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

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractString instance.
	template <typename... Arguments>
	AbstractString(const AllocatorType& allocator, AbstractStringView<CharacterType> format, Arguments&&... arguments);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AllocatorType& allocator = AllocatorType(), SizeType reservedLength = 0u);
	//!	Constructs this @ref AbstractString instance.
	template <typename InputIterator>
	AbstractString(const AllocatorType& allocator, InputIterator begin, InputIterator end);
	//!	Constructs this @ref AbstractString instance.
	template <typename Character2>
	AbstractString(const AllocatorType& allocator, AbstractStringView<Character2> string);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AllocatorType& allocator, AbstractStringView<CharacterType> string);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AllocatorType& allocator, const AbstractString&);
	//!	Constructs this @ref AbstractString instance.
	AbstractString(const AbstractString&) = default;
	//!	Constructs this @ref AbstractString instance.
	AbstractString(AbstractString&&) = default;

	~AbstractString() = default;

	// - ALLOCATOR ACCESS --------------------------------

public:
	const AllocatorType& GetAllocator() const;

	// ---------------------------------------------------

public:
	operator AbstractStringView<CharacterType>() const;

	operator const CharacterType*() const;

	const CharacterType* AsCString() const;

	const CharacterType* GetData() const;
	CharacterType*       GetData();

	// - ALGORITHMS --------------------------------------

public:
	SizeType Find(AbstractStringView<CharacterType> needle, SizeType offset = 0u) const;
	SizeType Find(CharacterType character, SizeType offset = 0u) const;

	SizeType FindLast(AbstractStringView<CharacterType> needle, SizeType offset = 0u) const;
	SizeType FindLast(CharacterType character, SizeType offset = 0u) const;

	bool Contains(AbstractStringView<CharacterType> string) const;
	bool Contains(CharacterType character) const;

	bool StartsWith(AbstractStringView<CharacterType> prefix) const;
	bool StartsWith(CharacterType prefix) const;

	bool EndsWith(AbstractStringView<CharacterType> suffix) const;
	bool EndsWith(CharacterType suffix) const;

	// ---------------------------------------------------

public:
	void MakeLowerCase();

	void MakeUpperCase();

	void ReplaceAll(CharacterType value, CharacterType replacement);

	// ---------------------------------------------------

public:
	ConstIterator Erase(ConstIterator begin, ConstIterator end);
	ConstIterator Erase(ConstIterator where);

	void Trim(SizeType charactersToAdvanceBeginning, SizeType charactersToRemoveFromEnd);
	void Trim(ConstIterator newBegin, ConstIterator newEnd);

	// - ELEMENT ITERATION -------------------------------

public:
	ConstIterator Begin() const;

	ConstIterator End() const;

	ConstReverseIterator ReverseBegin() const;

	ConstReverseIterator ReverseEnd() const;

	ConstIterator ConstBegin() const;

	ConstIterator ConstEnd() const;

	ConstReverseIterator ConstReverseBegin() const;

	ConstReverseIterator ConstReverseEnd() const;

	CharacterType Front() const;

	CharacterType Back() const;

	// - CONTAINER DUPLICATION ---------------------------

public:
	template <typename Character2>
	AbstractString& Assign(AbstractStringView<Character2> string);
	template <typename... Arguments>
	AbstractString& Assign(AbstractStringView<CharacterType> format, Arguments&&... arguments);
	AbstractString& Assign(AbstractStringView<CharacterType> string);

	template <typename Character2>
	AbstractString& operator=(AbstractStringView<Character2> string);
	AbstractString& operator=(AbstractStringView<CharacterType> string);
	AbstractString& operator=(const AbstractString&) = default;
	AbstractString& operator=(AbstractString&&) = default;

	// - CONTAINER MANIPULATION --------------------------

public:
	AbstractString& EnsurePrefix(AbstractStringView<CharacterType> prefix);
	AbstractString& EnsurePrefix(CharacterType prefix);

	AbstractString& EnsureSuffix(AbstractStringView<CharacterType> suffix);
	AbstractString& EnsureSuffix(CharacterType suffix);

	AbstractString& Prepend(AbstractStringView<CharacterType> string);
	AbstractString& Prepend(CharacterType value);

	template <typename Character2>
	AbstractString& Append(AbstractStringView<Character2> string);
	template <typename... Arguments>
	AbstractString& Append(AbstractStringView<CharacterType> format, Arguments&&... arguments);
	AbstractString& Append(AbstractStringView<CharacterType> string);
	template <typename InputIterator>
	AbstractString& Append(InputIterator begin, InputIterator end);
	AbstractString& Append(CharacterType value);

	AbstractString& operator+=(AbstractStringView<CharacterType> string);
	template <typename Character2>
	AbstractString& operator+=(AbstractStringView<Character2> string);
	AbstractString& operator+=(CharacterType value);

	// ---------------------------------------------------

public:
	void ForceSize(SizeType size);

	void Resize(SizeType size);

	void Clear();

	// - CONTAINER COMPARISON ----------------------------

public:
	int Compare(AbstractStringView<CharacterType> string) const;

	// - ELEMENT ACCESS ----------------------------------

public:
	CharacterType operator[](SizeType offset) const;

	// - CONTENT QUERY -----------------------------------

public:
	SizeType GetLength() const;

	bool IsEmpty() const;

	explicit operator bool() const;

	// - CAPACITY QUERY ----------------------------------

public:
	SizeType GetCapacityInCharacters() const;

	void Reserve(SizeType capacityInCharacters);

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
