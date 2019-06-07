/*==================================================================*\
  AbstractStringView.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <fmt/format.h>
//------------------------------------------------------------------//
#include <eastl/internal/char_traits.h>
#include <charconv>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename T>
class Hash;

template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Minimum(T, U...) ETNoexceptHint;

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t sizeInBytes, size_t seed = 0u) ETNoexceptHint;

} // namespace Eldritch2

namespace Eldritch2 {

template <typename Character>
ETConstexpr ETForceInlineHint AbstractStringSpan<Character>::AbstractStringSpan(const CharacterType* begin, SizeType length) ETNoexceptHint : _view(begin, length) {}

// ---------------------------------------------------

template <typename Character>
template <size_t size>
ETConstexpr ETForceInlineHint AbstractStringSpan<Character>::AbstractStringSpan(const CharacterType (&string)[size]) ETNoexceptHint : _view(string, SizeType(size - 1)) {}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstIterator AbstractStringSpan<Character>::Begin() const ETNoexceptHint {
	return _view.begin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstIterator AbstractStringSpan<Character>::End() const ETNoexceptHint {
	return _view.end();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstIterator AbstractStringSpan<Character>::ConstBegin() const ETNoexceptHint {
	return _view.cbegin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstIterator AbstractStringSpan<Character>::ConstEnd() const ETNoexceptHint {
	return _view.cend();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstReverseIterator AbstractStringSpan<Character>::ReverseBegin() const ETNoexceptHint {
	return _view.rbegin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstReverseIterator AbstractStringSpan<Character>::ReverseEnd() const ETNoexceptHint {
	return _view.rend();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstReverseIterator AbstractStringSpan<Character>::ConstReverseBegin() const ETNoexceptHint {
	return _view.crbegin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstReverseIterator AbstractStringSpan<Character>::ConstReverseEnd() const ETNoexceptHint {
	return _view.crend();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename const AbstractStringSpan<Character>::CharacterType* AbstractStringSpan<Character>::GetData() const ETNoexceptHint {
	return _view.data();
}

// ----------------------------------------------------

template <typename Character>
template <typename Value>
ETInlineHint ETForceInlineHint bool AbstractStringSpan<Character>::Consume(Value& value) ETNoexceptHint {
	return int(std::from_chars(_view.begin(), _view.end(), value).ec) == 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::CharacterType AbstractStringSpan<Character>::Front() const ETNoexceptHint {
	return _view.front();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::CharacterType AbstractStringSpan<Character>::Back() const ETNoexceptHint {
	return _view.back();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::CharacterType AbstractStringSpan<Character>::operator[](SizeType offset) const ETNoexceptHint {
	return _view[offset];
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint int AbstractStringSpan<Character>::Compare(const AbstractStringSpan<Character>& string) const ETNoexceptHint {
	return _view.compare(string._view);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint int AbstractStringSpan<Character>::Compare(const CharacterType* string) const ETNoexceptHint {
	return _view.compare(string);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::GetLength() const ETNoexceptHint {
	return SizeType(_view.length());
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::GetSize() const ETNoexceptHint {
	return SizeType(_view.size());
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool AbstractStringSpan<Character>::IsEmpty() const ETNoexceptHint {
	return _view.empty();
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr bool AbstractStringSpan<Character>::StartsWith(const AbstractStringSpan<Character>& prefix) const ETNoexceptHint {
	return _view.starts_with(prefix._view);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr bool AbstractStringSpan<Character>::StartsWith(CharacterType character) const ETNoexceptHint {
	return _view.starts_with(character);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr bool AbstractStringSpan<Character>::EndsWith(const AbstractStringSpan<Character>& suffix) const ETNoexceptHint {
	return _view.ends_with(suffix._view);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr bool AbstractStringSpan<Character>::EndsWith(CharacterType character) const ETNoexceptHint {
	return _view.ends_with(character);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint bool AbstractStringSpan<Character>::RemovePrefix(const AbstractStringSpan<Character>& prefix) ETNoexceptHint {
	return this->StartsWith(prefix) ? (this->RemovePrefix(prefix.GetLength()), true) : false;
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint void AbstractStringSpan<Character>::RemovePrefix(SizeType length) ETNoexceptHint {
	_view.remove_prefix(length);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint bool AbstractStringSpan<Character>::RemoveSuffix(const AbstractStringSpan<Character>& suffix) ETNoexceptHint {
	return this->EndsWith(suffix) ? (this->RemoveSuffix(suffix.GetLength()), true) : false;
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint void AbstractStringSpan<Character>::RemoveSuffix(SizeType length) ETNoexceptHint {
	_view.remove_suffix(length);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::ConstSliceType AbstractStringSpan<Character>::Slice(SizeType offset, SizeType length) const ETNoexceptHint {
	return AbstractStringSpan<Character>(this->GetData() + offset, Eldritch2::Minimum(this->GetLength() - length, length));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::Find(const AbstractStringSpan<Character>& string, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find(string._view, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::Find(CharacterType character, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find(character, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindLast(const AbstractStringSpan<Character>& string, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.rfind(string._view, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindLast(CharacterType character, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.rfind(character, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindAny(const AbstractStringSpan<Character>& set, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find_first_of(set._view, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindAnyLast(const AbstractStringSpan<Character>& set, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find_last_of(set._view, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindNot(const AbstractStringSpan<Character>& set, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find_first_not_of(set._view, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindNot(CharacterType value, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find_first_not_of(value, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindNotLast(const AbstractStringSpan<Character>& set, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find_last_not_of(set._view, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::FindNotLast(CharacterType value, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.find_last_not_of(value, offset));
}

// ----------------------------------------------------

template <typename Character>
ETInlineHint ETForceInlineHint typename AbstractStringSpan<Character>::SizeType AbstractStringSpan<Character>::Copy(CharacterType* target, SizeType limit, SizeType offset) const ETNoexceptHint {
	return SizeType(_view.copy(target, limit, offset));
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint void Swap(AbstractStringSpan<Character>& lhs, AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	eastl::swap(lhs._view, rhs._view);
}

// ----------------------------------------------------

template <typename Value, typename Character2>
ETInlineHint ETForceInlineHint bool Parse(Value& value, const AbstractStringSpan<Character2>& string) ETNoexceptHint {
	return int(std::from_chars(string.Begin(), string.End(), value).ec) == 0;
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETForceInlineHint size_t GetHashCode(const AbstractStringSpan<Character>& string, size_t seed = 0u) ETNoexceptHint {
	return Eldritch2::HashMemory(string.Begin(), string.GetLength() * sizeof(Character), seed);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator==(const AbstractStringSpan<Character>& lhs, const AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) == 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator!=(const AbstractStringSpan<Character>& lhs, const AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator<(const AbstractStringSpan<Character>& lhs, const AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) < 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator<=(const AbstractStringSpan<Character>& lhs, const AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	return rhs.Compare(lhs) < 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator>(const AbstractStringSpan<Character>& lhs, const AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) > 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator>=(const AbstractStringSpan<Character>& lhs, const AbstractStringSpan<Character>& rhs) ETNoexceptHint {
	return rhs.Compare(lhs) > 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator==(const AbstractStringSpan<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) == 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator!=(const AbstractStringSpan<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator<(const AbstractStringSpan<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) < 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator<=(const AbstractStringSpan<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) <= 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator>(const AbstractStringSpan<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) > 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETForceInlineHint bool operator>=(const AbstractStringSpan<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) >= 0;
}

} // namespace Eldritch2

template <typename Character>
class Eldritch2::Hash<Eldritch2::AbstractStringSpan<Character>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	ETConstexpr Hash(const Hash&) ETNoexceptHint = default;
	//!	Constructs this @ref Hash instance.
	ETConstexpr Hash() ETNoexceptHint = default;

	~Hash() = default;

	// ---------------------------------------------------

public:
	ETCpp14Constexpr ETForceInlineHint ETPureFunctionHint size_t operator()(Eldritch2::AbstractStringSpan<Character> string, size_t seed = 0u) const ETNoexceptHint {
		return Eldritch2::GetHashCode(string, seed);
	}
};

template <typename Character, typename OutCharacter>
struct fmt::formatter<Eldritch2::AbstractStringSpan<Character>, OutCharacter> {
	template <typename ParseContext>
	ETConstexpr ETForceInlineHint auto parse(ParseContext& context) ETNoexceptHintIf(noexcept(context.begin())) -> decltype(context.begin()) {
		return context.begin();
	}

	template <typename FormatContext>
	ETInlineHint auto format(Eldritch2::AbstractStringSpan<Character> string, FormatContext& context) -> decltype(context.out()) {
		auto& buffer(internal::get_container(context.out()));
		buffer.reserve(buffer.size() + string.GetLength());
		return Eldritch2::TranscodeString<OutCharacter>(string.Begin(), string.End(), context.out());
	}
};
