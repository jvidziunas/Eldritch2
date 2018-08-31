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
class AbstractStringView {
	// - TYPE PUBLISHING ---------------------------------

private:
	using UnderlyingView = eastl::basic_string_view<Character>;

public:
	using ConstIterator        = typename UnderlyingView::const_iterator;
	using ConstReverseIterator = typename UnderlyingView::const_reverse_iterator;
	using CharacterType        = typename UnderlyingView::value_type;
	using ValueType            = typename UnderlyingView::value_type;
	using SizeType             = typename UnderlyingView::size_type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringView(const CharacterType* begin, SizeType length) ETNoexceptHint;
	//!	Constructs this @ref AbstractStringView instance.
	template <size_t size>
	ETConstexpr AbstractStringView(const CharacterType (&string)[size]) ETNoexceptHint;
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringView(const AbstractStringView&) ETNoexceptHint = default;
	//!	Constructs this @ref AbstractStringView instance.
	ETConstexpr AbstractStringView() ETNoexceptHint = default;

	~AbstractStringView() = default;

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
	ETConstexpr CharacterType Front() const ETNoexceptHint;

	ETConstexpr CharacterType Back() const ETNoexceptHint;

	ETConstexpr CharacterType operator[](SizeType offset) const ETNoexceptHint;

	// ----------------------------------------------------

public:
	ETConstexpr int Compare(const AbstractStringView& string) const ETNoexceptHint;
	ETConstexpr int Compare(const CharacterType* string) const ETNoexceptHint;

	// ----------------------------------------------------

public:
	ETCpp14Constexpr bool StartsWith(const AbstractStringView& prefix) const ETNoexceptHint;

	ETCpp14Constexpr bool EndsWith(const AbstractStringView& suffix) const ETNoexceptHint;

	ETCpp14Constexpr void RemovePrefix(const AbstractStringView& prefix) ETNoexceptHint;
	ETCpp14Constexpr void RemovePrefix(SizeType length) ETNoexceptHint;

	ETCpp14Constexpr void RemoveSuffix(const AbstractStringView& suffix) ETNoexceptHint;
	ETCpp14Constexpr void RemoveSuffix(SizeType length) ETNoexceptHint;

	ETCpp14Constexpr AbstractStringView Substring(SizeType offset, SizeType length = SizeType(-1)) const ETNoexceptHint;

	ETCpp14Constexpr SizeType Find(const AbstractStringView& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType Find(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindLast(const AbstractStringView& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType FindLast(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindAny(const AbstractStringView& string, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindAnyLast(const AbstractStringView& string, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindNot(const AbstractStringView& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType FindNot(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	ETCpp14Constexpr SizeType FindNotLast(const AbstractStringView& string, SizeType offset = 0) const ETNoexceptHint;
	ETCpp14Constexpr SizeType FindNotLast(CharacterType value, SizeType offset = 0) const ETNoexceptHint;

	SizeType Copy(CharacterType* target, SizeType limit, SizeType offset = 0) const ETNoexceptHint;

	// - SIZE QUERY --------------------------------------

public:
	ETConstexpr SizeType GetLength() const ETNoexceptHint;

	ETConstexpr SizeType GetSize() const ETNoexceptHint;

	ETConstexpr bool IsEmpty() const ETNoexceptHint;

	// ----------------------------------------------------

	template <typename Character>
	friend ETCpp14Constexpr void Swap(AbstractStringView<Character>&, AbstractStringView<Character>&) ETNoexceptHint;

	// - DATA MEMBERS ------------------------------------

private:
	UnderlyingView _view;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/AbstractStringView.inl>
//------------------------------------------------------------------//
