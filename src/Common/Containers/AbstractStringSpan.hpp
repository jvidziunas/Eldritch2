/*==================================================================*\
  StringView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#include <eastl/string_view.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Character>
class AbstractStringSpan {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingView = eastl::basic_string_view<Character>;

public:
	using ConstIterator        = typename UnderlyingView::const_iterator;
	using ConstReverseIterator = typename UnderlyingView::const_reverse_iterator;
	using CharacterType        = typename UnderlyingView::value_type;
	using ValueType            = typename UnderlyingView::value_type;
	using SizeType             = eastl_size_t;
	using SliceType            = AbstractStringSpan<CharacterType>;
	using ConstSliceType       = AbstractStringSpan<CharacterType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringSpan(const CharacterType* begin, SizeType length) ETNoexceptHint;
	//!	Constructs this @ref AbstractStringView instance.
	template <size_t size>
	ETConstexpr AbstractStringSpan(const CharacterType (&string)[size]) ETNoexceptHint;
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringSpan(const AbstractStringSpan&) ETNoexceptHint = default;
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringSpan(AbstractStringSpan&&) ETNoexceptHint = default;
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringSpan() ETNoexceptHint = default;

	~AbstractStringSpan() = default;

	// ----------------------------------------------------

public:
	ETCpp14Constexpr SizeType Find(const AbstractStringSpan& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType Find(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindLast(const AbstractStringSpan& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType FindLast(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindAny(const AbstractStringSpan& string, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindAnyLast(const AbstractStringSpan& string, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindNot(const AbstractStringSpan& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType FindNot(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindNotLast(const AbstractStringSpan& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType FindNotLast(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	// - ELEMENT ITERATION -------------------------------

public:
	ETConstexpr ConstIterator Begin() const ETNoexceptHint;

	ETConstexpr ConstIterator End() const ETNoexceptHint;

	ETConstexpr ConstIterator ConstBegin() const ETNoexceptHint;

	ETConstexpr ConstIterator ConstEnd() const ETNoexceptHint;

	ETConstexpr ConstReverseIterator ReverseBegin() const ETNoexceptHint;

	ETConstexpr ConstReverseIterator ReverseEnd() const ETNoexceptHint;

	ETConstexpr ConstReverseIterator ConstReverseBegin() const ETNoexceptHint;

	ETConstexpr ConstReverseIterator ConstReverseEnd() const ETNoexceptHint;

	ETConstexpr const CharacterType* GetData() const ETNoexceptHint;

	// ----------------------------------------------------

public:
	template <typename Value>
	bool Consume(Value& value) ETNoexceptHint;

	// ----------------------------------------------------

public:
	ETConstexpr CharacterType Front() const ETNoexceptHint;

	ETConstexpr CharacterType Back() const ETNoexceptHint;

	ETConstexpr CharacterType operator[](SizeType offset) const ETNoexceptHint;

	// ----------------------------------------------------

public:
	ETConstexpr int CompareCaseInsensitive(const AbstractStringSpan& string) const ETNoexceptHint;
	ETConstexpr int CompareCaseInsensitive(const CharacterType* string) const ETNoexceptHint;

	ETConstexpr int Compare(const AbstractStringSpan& string) const ETNoexceptHint;
	ETConstexpr int Compare(const CharacterType* string) const ETNoexceptHint;

	// ----------------------------------------------------

public:
	ETCpp14Constexpr bool StartsWith(const AbstractStringSpan& prefix) const ETNoexceptHint;
	ETCpp14Constexpr bool StartsWith(CharacterType character) const ETNoexceptHint;

	ETCpp14Constexpr bool EndsWith(const AbstractStringSpan& suffix) const ETNoexceptHint;
	ETCpp14Constexpr bool EndsWith(CharacterType character) const ETNoexceptHint;

	ETCpp14Constexpr bool RemovePrefix(const AbstractStringSpan& prefix) ETNoexceptHint;
	ETCpp14Constexpr void RemovePrefix(SizeType length) ETNoexceptHint;

	ETCpp14Constexpr bool RemoveSuffix(const AbstractStringSpan& suffix) ETNoexceptHint;
	ETCpp14Constexpr void RemoveSuffix(SizeType length) ETNoexceptHint;

	ETCpp14Constexpr ConstSliceType Slice(SizeType offset, SizeType length = SizeType(-1)) const ETNoexceptHint;

	SizeType Copy(CharacterType* target, SizeType limit, SizeType offset = 0) const ETNoexceptHint;

	// - SIZE QUERY --------------------------------------

public:
	ETConstexpr SizeType GetLength() const ETNoexceptHint;

	ETConstexpr SizeType GetSize() const ETNoexceptHint;

	ETConstexpr bool IsEmpty() const ETNoexceptHint;

	// ----------------------------------------------------

public:
	AbstractStringSpan& operator=(const AbstractStringSpan&) ETNoexceptHint = default;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingView _view;

	// ----------------------------------------------------

	template <typename Character2>
	friend ETCpp14Constexpr void Swap(AbstractStringSpan<Character2>&, AbstractStringSpan<Character2>&) ETNoexceptHint;

	template <typename Value, typename Character2>
	friend bool Parse(Value& value, const AbstractStringSpan<Character2>& string) ETNoexceptHint;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/AbstractStringSpan.inl>
//------------------------------------------------------------------//
