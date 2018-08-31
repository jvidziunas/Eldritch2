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
//------------------------------------------------------------------//

namespace Eldritch2 {
template <typename T>
class Hash;

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t sizeInBytes, size_t seed = 0u) ETNoexceptHint;
} // namespace Eldritch2

namespace Eldritch2 {

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint AbstractStringView<Character>::AbstractStringView(const CharacterType* begin, SizeType length) ETNoexceptHint : _view(begin, length) {}

// ---------------------------------------------------

template <typename Character>
template <size_t size>
ETConstexpr ETInlineHint ETForceInlineHint AbstractStringView<Character>::AbstractStringView(const CharacterType (&string)[size]) ETNoexceptHint : _view(string, size - 1) {}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstIterator AbstractStringView<Character>::Begin() const ETNoexceptHint {
	return _view.begin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstIterator AbstractStringView<Character>::End() const ETNoexceptHint {
	return _view.end();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstIterator AbstractStringView<Character>::ConstBegin() const ETNoexceptHint {
	return _view.cbegin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstIterator AbstractStringView<Character>::ConstEnd() const ETNoexceptHint {
	return _view.cend();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstReverseIterator AbstractStringView<Character>::ReverseBegin() const ETNoexceptHint {
	return _view.rbegin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstReverseIterator AbstractStringView<Character>::ReverseEnd() const ETNoexceptHint {
	return _view.rend();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstReverseIterator AbstractStringView<Character>::ConstReverseBegin() const ETNoexceptHint {
	return _view.crbegin();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::ConstReverseIterator AbstractStringView<Character>::ConstReverseEnd() const ETNoexceptHint {
	return _view.crend();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename const AbstractStringView<Character>::CharacterType* AbstractStringView<Character>::GetData() const ETNoexceptHint {
	return _view.data();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::CharacterType AbstractStringView<Character>::Front() const ETNoexceptHint {
	return _view.front();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::CharacterType AbstractStringView<Character>::Back() const ETNoexceptHint {
	return _view.back();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::CharacterType AbstractStringView<Character>::operator[](SizeType offset) const ETNoexceptHint {
	return _view[offset];
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint int AbstractStringView<Character>::Compare(const CharacterType* string) const ETNoexceptHint {
	return _view.compare(string);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint int AbstractStringView<Character>::Compare(const AbstractStringView<Character>& string) const ETNoexceptHint {
	return _view.compare(string._view);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::GetLength() const ETNoexceptHint {
	return _view.length();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::GetSize() const ETNoexceptHint {
	return _view.size();
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool AbstractStringView<Character>::IsEmpty() const ETNoexceptHint {
	return _view.empty();
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr bool AbstractStringView<Character>::StartsWith(const AbstractStringView<Character>& prefix) const ETNoexceptHint {
	return prefix.GetLength() <= this->GetLength() ? Substring(0, prefix.GetLength()).Compare(prefix) == 0 : false;
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr bool AbstractStringView<Character>::EndsWith(const AbstractStringView<Character>& suffix) const ETNoexceptHint {
	return suffix.GetLength() <= this->GetLength() ? Substring(this->GetLength() - suffix.GetLength(), suffix.GetLength()).Compare(suffix) == 0 : false;
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint void AbstractStringView<Character>::RemovePrefix(const AbstractStringView<Character>& prefix) ETNoexceptHint {
	if (StartsWith(prefix)) {
		RemovePrefix(prefix.GetLength());
	}
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint void AbstractStringView<Character>::RemovePrefix(SizeType length) ETNoexceptHint {
	_view.remove_prefix(length);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint void AbstractStringView<Character>::RemoveSuffix(const AbstractStringView<Character>& suffix) ETNoexceptHint {
	if (EndsWith(suffix)) {
		RemoveSuffix(suffix.GetLength());
	}
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint void AbstractStringView<Character>::RemoveSuffix(SizeType length) ETNoexceptHint {
	_view.remove_suffix(length);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint AbstractStringView<Character> AbstractStringView<Character>::Substring(SizeType offset, SizeType length) const ETNoexceptHint {
	const UnderlyingView view(_view.substr(offset, length));
	return { view.data(), view.length() };
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::Find(const AbstractStringView<Character>& string, SizeType offset) const ETNoexceptHint {
	return _view.find(string._view, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::Find(CharacterType character, SizeType offset) const ETNoexceptHint {
	return _view.find(character, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindLast(const AbstractStringView<Character>& string, SizeType offset) const ETNoexceptHint {
	return _view.rfind(string._view, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindLast(CharacterType character, SizeType offset) const ETNoexceptHint {
	return _view.rfind(character, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindAny(const AbstractStringView<Character>& set, SizeType offset) const ETNoexceptHint {
	return _view.find_first_of(set._view, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindAnyLast(const AbstractStringView<Character>& set, SizeType offset) const ETNoexceptHint {
	return _view.find_last_of(set._view, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindNot(const AbstractStringView<Character>& set, SizeType offset) const ETNoexceptHint {
	return _view.find_first_not_of(set._view, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindNot(CharacterType value, SizeType offset) const ETNoexceptHint {
	return _view.find_first_not_of(value, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindNotLast(const AbstractStringView<Character>& set, SizeType offset) const ETNoexceptHint {
	return _view.find_last_not_of(set._view, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::FindNotLast(CharacterType value, SizeType offset) const ETNoexceptHint {
	return _view.find_last_not_of(value, offset);
}

// ----------------------------------------------------

template <typename Character>
ETInlineHint ETForceInlineHint typename AbstractStringView<Character>::SizeType AbstractStringView<Character>::Copy(CharacterType* target, SizeType limit, SizeType offset) const ETNoexceptHint {
	return _view.copy(target, limit, offset);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint void Swap(AbstractStringView<Character>& lhs, AbstractStringView<Character>& rhs) ETNoexceptHint {
	eastl::swap(lhs._view, rhs._view);
}

// ----------------------------------------------------

template <typename Character>
ETCpp14Constexpr ETInlineHint ETForceInlineHint size_t GetHashCode(const AbstractStringView<Character>& string, size_t seed = 0u) ETNoexceptHint {
	return Eldritch2::HashMemory(string.Begin(), string.GetLength() * sizeof(Character), seed);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator==(const AbstractStringView<Character>& lhs, const AbstractStringView<Character>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) == 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator!=(const AbstractStringView<Character>& lhs, const AbstractStringView<Character>& rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator<(const AbstractStringView<Character>& lhs, const AbstractStringView<Character>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) < 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator<=(const AbstractStringView<Character>& lhs, const AbstractStringView<Character>& rhs) ETNoexceptHint {
	return rhs.Compare(lhs) < 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator>(const AbstractStringView<Character>& lhs, const AbstractStringView<Character>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) > 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator>=(const AbstractStringView<Character>& lhs, const AbstractStringView<Character>& rhs) ETNoexceptHint {
	return rhs.Compare(lhs) > 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator==(const AbstractStringView<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) == 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator!=(const AbstractStringView<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return !(lhs == rhs);
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator<(const AbstractStringView<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) < 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator<=(const AbstractStringView<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) <= 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator>(const AbstractStringView<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) > 0;
}

// ----------------------------------------------------

template <typename Character>
ETConstexpr ETInlineHint ETForceInlineHint bool operator>=(const AbstractStringView<Character>& lhs, const Character* rhs) ETNoexceptHint {
	return lhs.Compare(rhs) >= 0;
}

} // namespace Eldritch2

template <typename Character>
class ::Eldritch2::Hash<::Eldritch2::AbstractStringView<Character>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash(const Hash&) ETNoexceptHint = default;
	//!	Constructs this @ref Hash instance.
	Hash() ETNoexceptHint = default;

	~Hash() = default;

	// ---------------------------------------------------

public:
	ETCpp14Constexpr ETInlineHint ETForceInlineHint ETPureFunctionHint size_t operator()(::Eldritch2::AbstractStringView<Character> string, size_t seed = 0u) const ETNoexceptHint {
		return ::Eldritch2::GetHashCode(string, seed);
	}
};

template <typename Character>
struct ::fmt::v5::formatter<::Eldritch2::AbstractStringView<Character>, Character> {
	template <typename ParseContext>
	ETConstexpr ETForceInlineHint auto parse(ParseContext& context) -> decltype(eastl::declval<ParseContext>().begin()) {
		return context.begin();
	}

	template <typename FormatContext>
	auto format(::Eldritch2::AbstractStringView<Character> string, FormatContext& context) -> decltype(eastl::declval<FormatContext>().begin()) {
		fmt::internal::basic_buffer<Character>& buffer(fmt::internal::get_container(context.out()));
		buffer.reserve(buffer.capacity() + string.GetLength());
		return ::Eldritch2::Copy(string.Begin(), string.End(), context.begin());
	}
};

template <>
struct ::fmt::v5::formatter<::Eldritch2::AbstractStringView<::Eldritch2::PlatformChar>, ::Eldritch2::Utf8Char, std::enable_if_t<!eastl::is_same_v<::Eldritch2::Utf8Char, ::Eldritch2::PlatformChar>>> {
	template <typename ParseContext>
	ETConstexpr ETForceInlineHint auto parse(ParseContext& context) -> decltype(eastl::declval<ParseContext>().begin()) {
		return context.begin();
	}

	template <typename FormatContext>
	auto format(const ::Eldritch2::AbstractStringView<::Eldritch2::PlatformChar>& string, FormatContext& context) -> decltype(eastl::declval<FormatContext>().begin()) {
		::Eldritch2::Utf8Char staging[32u / sizeof(::Eldritch2::Utf8Char)];
		auto                  output(context.begin());

		for (auto input(string.ConstBegin()), end(string.ConstEnd()); input < end;) {
			auto lastDecoded(eastl::begin(staging));
			eastl::DecodePart(input, end, lastDecoded, eastl::end(staging));
			output = eastl::copy(eastl::begin(staging), lastDecoded, output);
		}

		return output;
	}
};

template <>
struct ::fmt::v5::formatter<::Eldritch2::AbstractStringView<::Eldritch2::Utf8Char>, ::Eldritch2::PlatformChar, std::enable_if_t<!eastl::is_same_v<::Eldritch2::Utf8Char, ::Eldritch2::PlatformChar>>> {
	template <typename ParseContext>
	ETConstexpr ETForceInlineHint auto parse(ParseContext& context) -> decltype(eastl::declval<ParseContext>().begin()) {
		return context.begin();
	}

	template <typename FormatContext>
	auto format(::Eldritch2::AbstractStringView<::Eldritch2::Utf8Char> string, FormatContext& context) -> decltype(eastl::declval<FormatContext>().begin()) {
		::Eldritch2::PlatformChar staging[32u / sizeof(::Eldritch2::PlatformChar)];
		auto                      output(context.begin());

		for (auto input(string.ConstBegin()), end(string.ConstEnd()); input < end;) {
			auto lastDecoded(eastl::begin(staging));
			eastl::DecodePart(input, end, lastDecoded, eastl::end(staging));
			output = eastl::copy(eastl::begin(staging), lastDecoded, output);
		}

		return output;
	}
};
