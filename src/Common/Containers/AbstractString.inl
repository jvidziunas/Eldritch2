/*==================================================================*\
  AbstractString.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/RangeAdapters.hpp>
#include <Common/Algorithms.hpp>
//------------------------------------------------------------------//
ET_PUSH_MSVC_WARNING_STATE(disable : 4459 4244)
#include <fmt/format.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
template <typename T>
class Hash;

template <typename T>
class EqualTo;

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t sizeInBytes, size_t seed) ETNoexceptHint;
} // namespace Eldritch2

namespace Eldritch2 {

template <typename Character, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, AbstractStringView<CharacterType> format, Arguments&&... arguments) :
	_container(allocator) {
	Assign(format, eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, AbstractStringView<Character2> string) :
	_container(UnderlyingContainer::CtorConvert(), string.ConstBegin(), string.GetLength(), allocator) {
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, AbstractStringView<CharacterType> string) :
	_container(string.Begin(), string.End(), allocator) {
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, const AbstractString<Character, Allocator>& string) :
	_container(string.Begin(), string.End(), allocator) {
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, InputIterator begin, InputIterator end) :
	_container(begin, end, allocator) {
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, SizeType reservedLength) :
	_container(UnderlyingContainer::CtorDoNotInitialize(), reservedLength, allocator) {
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::Find(AbstractStringView<CharacterType> needle, SizeType offset) const {
	return _container.find(needle.Begin(), offset, needle.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::Find(CharacterType value, SizeType offset) const {
	return _container.find(value, offset);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::FindLast(AbstractStringView<CharacterType> needle, SizeType offset) const {
	return _container.rfind(needle.Begin(), offset, needle.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::FindLast(CharacterType value, SizeType offset) const {
	return _container.rfind(value, offset);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::Contains(AbstractStringView<CharacterType> needle) const {
	return this->Find(needle) != SizeType(-1);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::Contains(CharacterType value) const {
	return this->Find(value) != SizeType(-1);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::MakeLowerCase() {
	_container.make_lower();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::MakeUpperCase() {
	_container.make_upper();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::ReplaceAll(CharacterType value, CharacterType replacement) {
	for (SizeType position(_container.find(value, 0u)); position != SizeType(-1); position = _container.find(value, position)) {
		_container[position++] = replacement;
	}
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::StartsWith(AbstractStringView<CharacterType> needle) const {
	return needle.GetLength() <= this->GetLength() ? StringsEqual(needle.GetData(), this->GetData(), needle.GetLength()) : false;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::StartsWith(CharacterType value) const {
	return bool(*this) ? this->Front() == value : false;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::EndsWith(AbstractStringView<CharacterType> needle) const {
	return needle.GetLength() <= this->GetLength() ? StringsEqual(needle.GetData(), this->End() - needle.GetLength(), needle.GetLength()) : false;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::EndsWith(CharacterType character) const {
	return bool(*this) ? this->Back() == character : false;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Erase(ConstIterator begin, ConstIterator end) {
	return _container.erase(begin, end);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Erase(ConstIterator position) {
	return _container.erase(position);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Begin() const {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::End() const {
	return _container.end();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ReverseBegin() const {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ReverseEnd() const {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::ConstBegin() const {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::ConstEnd() const {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ConstReverseBegin() const {
	return _container.crbegin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ConstReverseEnd() const {
	return _container.crend();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::CharacterType AbstractString<Character, Allocator>::Front() const {
	return _container.front();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::CharacterType AbstractString<Character, Allocator>::Back() const {
	return _container.back();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator=(AbstractStringView<CharacterType> string) {
	_container.assign(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign(AbstractStringView<CharacterType> format, Arguments&&... arguments) {
	Clear();
	return Append(format, eastl::forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign(AbstractStringView<Character2> string) {
	_container.assign_convert(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign(AbstractStringView<CharacterType> string) {
	_container.assign(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsurePrefix(AbstractStringView<CharacterType> prefix) {
	return StartsWith(prefix) ? *this : Prepend(prefix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsurePrefix(CharacterType prefix) {
	return StartsWith(prefix) ? *this : Prepend(prefix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsureSuffix(AbstractStringView<CharacterType> suffix) {
	return EndsWith(suffix) ? *this : Append(suffix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsureSuffix(CharacterType suffix) {
	return EndsWith(suffix) ? *this : Append(suffix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Prepend(AbstractStringView<CharacterType> string) {
	_container.insert(/*position =*/SizeType(0u), string.Begin(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Prepend(CharacterType value) {
	_container.insert(/*position =*/SizeType(0u), /*n =*/SizeType(1u), value);
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(AbstractStringView<CharacterType> format, Arguments&&... arguments) {
	fmt::format_to(std::back_inserter(_container), fmt::basic_string_view<CharacterType>(format.GetData(), format.GetLength()), arguments...);
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(AbstractStringView<Character2> string) {
	_container.append_convert(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(AbstractStringView<CharacterType> string) {
	_container.append(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename InputIterator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(InputIterator begin, InputIterator end) {
	_container.append_convert(begin, SizeType(eastl::distance(begin, end)));
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(CharacterType value) {
	_container.push_back(value);
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=(AbstractStringView<Character2> string) {
	_container.append_convert(string.Begin(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=(AbstractStringView<CharacterType> string) {
	_container.append(string.Begin(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=(CharacterType value) {
	_container.push_back(value);
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Clear() {
	_container.clear();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Trim(SizeType charactersToAdvanceBeginning, SizeType charactersToRemoveFromEnd) {
	_container.erase(_container.end() - charactersToRemoveFromEnd, _container.end());
	_container.erase(_container.begin(), _container.begin() + charactersToAdvanceBeginning);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Trim(ConstIterator newBegin, ConstIterator newEnd) {
	_container.erase(newEnd, _container.end());
	_container.erase(_container.begin(), newBegin);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::ForceSize(SizeType size) {
	_container.force_size(size);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Resize(SizeType size) {
	_container.resize(size);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint int AbstractString<Character, Allocator>::Compare(AbstractStringView<CharacterType> string) const {
	const intptr diff(intptr(GetLength()) - intptr(string.GetLength()));
	return diff < 0 ? -1 : diff > 0 ? 1 : OrderStrings(*this, string.GetData(), string.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::CharacterType AbstractString<Character, Allocator>::operator[](SizeType offset) const {
	return _container[offset];
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::operator AbstractStringView<typename AbstractString<Character, Allocator>::CharacterType>() const {
	return { _container.c_str(), _container.length() };
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::operator typename const AbstractString<Character, Allocator>::CharacterType*() const {
	return _container.c_str();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename const AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::AsCString() const {
	return _container.c_str();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename const AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::GetData() const {
	return _container.data();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::GetData() {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::GetLength() const {
	return _container.length();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::IsEmpty() const {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::operator bool() const {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Reserve(SizeType capacityInCharacters) {
	_container.reserve(capacityInCharacters);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::GetCapacityInCharacters() const {
	return _container.capacity();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint const typename AbstractString<Character, Allocator>::AllocatorType& AbstractString<Character, Allocator>::GetAllocator() const {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const AbstractString<Character, StringAllocator>& lhs, const Character* const rhs) {
	return lhs.Compare(rhs) == 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const AbstractString<Character, StringAllocator>& lhs, AbstractStringView<Character> rhs) {
	return lhs.Compare(rhs) == 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const AbstractString<Character, StringAllocator>& lhs, const AbstractString<Character, StringAllocator>& rhs) {
	return lhs.Compare(rhs) != 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const AbstractString<Character, StringAllocator>& lhs, const Character* const rhs) {
	return lhs.Compare(rhs) != 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetHashCode(const AbstractString<Character, StringAllocator>& string, size_t seed) {
	return Eldritch2::HashMemory(string.AsCString(), string.GetLength() * sizeof(Character), seed);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void Swap(AbstractString<Character, Allocator>& lhs, AbstractString<Character, Allocator>& rhs) {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2

template <typename Character, class Allocator>
class ::Eldritch2::EqualTo<::Eldritch2::AbstractString<Character, Allocator>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref EqualTo instance.
	EqualTo(const EqualTo&) = default;
	//!	Constructs this @ref EqualTo instance.
	EqualTo() = default;

	~EqualTo() = default;

	// ---------------------------------------------------

public:
	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator()(const ::Eldritch2::AbstractString<Character, Allocator>& lhs, const ::Eldritch2::AbstractString<Character, Allocator>& rhs) const {
		return lhs == rhs;
	}

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator()(const ::Eldritch2::AbstractString<Character, Allocator>& lhs, AbstractStringView<Character> rhs) const {
		return lhs == rhs;
	}
};

template <typename Character, class Allocator>
class ::Eldritch2::Hash<::Eldritch2::AbstractString<Character, Allocator>> : public ::Eldritch2::Hash<::Eldritch2::AbstractStringView<Character>> {};

template <typename Character, typename Allocator>
struct ::fmt::v5::is_contiguous<::eastl::basic_string<Character, Allocator>> : public ::eastl::true_type {};

template <typename Character, class Allocator, typename OutCharacter>
struct ::fmt::v5::formatter<::Eldritch2::AbstractString<Character, Allocator>, OutCharacter> : public ::fmt::v5::formatter<::Eldritch2::AbstractStringView<Character>, OutCharacter> {};
