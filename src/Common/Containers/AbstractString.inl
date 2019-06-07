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
class EqualTo;
template <typename T>
class Hash;

template <typename T, typename... U>
ETCpp14Constexpr ETPureFunctionHint ETNeverThrowsHint T Minimum(T, U...) ETNoexceptHint;

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t byteSize, size_t seed) ETNoexceptHint;

} // namespace Eldritch2

namespace Eldritch2 {

template <typename Character, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, AbstractStringSpan<CharacterType> format, Arguments&&... arguments) :
	_container(allocator) {
	Assign(format, Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::AbstractString(const AllocatorType& allocator, AbstractStringSpan<Character2> string) :
	_container(UnderlyingContainer::CtorConvert(), string.ConstBegin(), string.GetLength(), allocator) {
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
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::Find(AbstractStringSpan<CharacterType> string, SizeType offset) const ETNoexceptHint {
	return _container.find(string.Begin(), offset, string.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::Find(CharacterType value, SizeType offset) const ETNoexceptHint {
	return _container.find(value, offset);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::FindLast(AbstractStringSpan<CharacterType> string, SizeType offset) const ETNoexceptHint {
	return _container.rfind(string.Begin(), offset, string.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::FindLast(CharacterType value, SizeType offset) const ETNoexceptHint {
	return _container.rfind(value, offset);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::Contains(AbstractStringSpan<CharacterType> string) const ETNoexceptHint {
	return this->Find(string) != SizeType(-1);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::Contains(CharacterType value) const ETNoexceptHint {
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
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::StartsWith(AbstractStringSpan<CharacterType> string) const ETNoexceptHint {
	return (string.GetLength() <= this->GetLength()) && Eldritch2::StringsEqual(string.GetData(), this->GetData(), string.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::StartsWith(CharacterType value) const ETNoexceptHint {
	return !this->IsEmpty() && this->Front() == value;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::EndsWith(AbstractStringSpan<CharacterType> string) const ETNoexceptHint {
	return (string.GetLength() <= this->GetLength()) && Eldritch2::StringsEqual(string.GetData(), this->End() - string.GetLength(), string.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::EndsWith(CharacterType value) const ETNoexceptHint {
	return !this->IsEmpty() && this->Back() == value;
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
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Begin() const ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::End() const ETNoexceptHint {
	return _container.end();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ReverseBegin() const ETNoexceptHint {
	return _container.rbegin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ReverseEnd() const ETNoexceptHint {
	return _container.rend();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::ConstBegin() const ETNoexceptHint {
	return _container.cbegin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::ConstEnd() const ETNoexceptHint {
	return _container.cend();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ConstReverseBegin() const ETNoexceptHint {
	return _container.crbegin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::ConstReverseIterator AbstractString<Character, Allocator>::ConstReverseEnd() const ETNoexceptHint {
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
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator=(AbstractStringSpan<CharacterType> string) {
	_container.assign(string.ConstBegin(), string.ConstEnd());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename... Arguments>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign(AbstractStringSpan<CharacterType> format, Arguments&&... arguments) {
	Clear();
	return Append(format, Forward<Arguments>(arguments)...);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign(AbstractStringSpan<Character2> string) {
	_container.assign_convert(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign(AbstractStringSpan<CharacterType> string) {
	_container.assign(string.ConstBegin(), string.ConstEnd());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsurePrefix(AbstractStringSpan<CharacterType> prefix) {
	return StartsWith(prefix) ? *this : Prepend(prefix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsurePrefix(CharacterType prefix) {
	return StartsWith(prefix) ? *this : Prepend(prefix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsureSuffix(AbstractStringSpan<CharacterType> suffix) {
	return EndsWith(suffix) ? *this : Append(suffix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsureSuffix(CharacterType suffix) {
	return EndsWith(suffix) ? *this : Append(suffix);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Prepend(AbstractStringSpan<CharacterType> string) {
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
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(AbstractStringSpan<CharacterType> format, Arguments&&... arguments) {
	fmt::format_to(std::back_inserter(_container), fmt::basic_string_view<CharacterType>(format.GetData(), format.GetLength()), arguments...);
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
template <typename Character2>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(AbstractStringSpan<Character2> string) {
	_container.append_convert(string.GetData(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append(AbstractStringSpan<CharacterType> string) {
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
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=(AbstractStringSpan<Character2> string) {
	_container.append_convert(string.Begin(), string.GetLength());
	return *this;
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=(AbstractStringSpan<CharacterType> string) {
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
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Clear() ETNoexceptHint {
	_container.clear();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Trim(SizeType charactersToAdvance, SizeType charactersToRemove) {
	_container.erase(_container.end() - charactersToRemove, _container.end());
	_container.erase(_container.begin(), _container.begin() + charactersToAdvance);
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
ETInlineHint ETForceInlineHint int AbstractString<Character, Allocator>::Compare(AbstractStringSpan<CharacterType> string) const ETNoexceptHint {
	const intptr diff(intptr(GetLength()) - intptr(string.GetLength()));
	return diff < 0 ? -1 : diff > 0 ? 1 : Eldritch2::OrderStrings(*this, string.GetData(), string.GetLength());
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::CharacterType AbstractString<Character, Allocator>::operator[](SizeType offset) const {
	return _container[offset];
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::operator AbstractStringSpan<typename AbstractString<Character, Allocator>::CharacterType>() const ETNoexceptHint {
	return { _container.c_str(), _container.length() };
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::operator typename const AbstractString<Character, Allocator>::CharacterType*() const ETNoexceptHint {
	return _container.c_str();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractStringSpan<typename AbstractString<Character, Allocator>::CharacterType> AbstractString<Character, Allocator>::Slice(SizeType offset, SizeType length) const ETNoexceptHint {
	return AbstractStringSpan<CharacterType>(_container.data() + offset, Minimum(_container.length() - offset, length));
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename const AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::AsCString() const ETNoexceptHint {
	return _container.c_str();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename const AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::GetData() const ETNoexceptHint {
	return _container.data();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::GetData() ETNoexceptHint {
	return _container.begin();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::GetLength() const ETNoexceptHint {
	return _container.length();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint bool AbstractString<Character, Allocator>::IsEmpty() const ETNoexceptHint {
	return _container.empty();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint AbstractString<Character, Allocator>::operator bool() const ETNoexceptHint {
	return !_container.empty();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void AbstractString<Character, Allocator>::Reserve(SizeType capacityInCharacters) {
	_container.reserve(capacityInCharacters);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::GetCapacityInCharacters() const ETNoexceptHint {
	return _container.capacity();
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint const typename AbstractString<Character, Allocator>::AllocatorType& AbstractString<Character, Allocator>::GetAllocator() const ETNoexceptHint {
	return _container.get_allocator();
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const AbstractString<Character, StringAllocator>& lhs, const Character* const rhs) ETNoexceptHint {
	return lhs.Compare(rhs) == 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator==(const AbstractString<Character, StringAllocator>& lhs, AbstractStringSpan<Character> rhs) ETNoexceptHint {
	return lhs.Compare(rhs) == 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const AbstractString<Character, StringAllocator>& lhs, const AbstractString<Character, StringAllocator>& rhs) ETNoexceptHint {
	return lhs.Compare(rhs) != 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator!=(const AbstractString<Character, StringAllocator>& lhs, const Character* const rhs) ETNoexceptHint {
	return lhs.Compare(rhs) != 0;
}

// ---------------------------------------------------

template <typename Character, class StringAllocator>
ETInlineHint ETForceInlineHint ETPureFunctionHint size_t GetHashCode(const AbstractString<Character, StringAllocator>& string, size_t seed) ETNoexceptHint {
	return Eldritch2::HashMemory(string.AsCString(), string.GetLength() * sizeof(Character), seed);
}

// ---------------------------------------------------

template <typename Character, class Allocator>
ETInlineHint ETForceInlineHint void Swap(AbstractString<Character, Allocator>& lhs, AbstractString<Character, Allocator>& rhs) ETNoexceptHint {
	lhs._container.swap(rhs._container);
}

} // namespace Eldritch2

template <typename Character, class Allocator>
class Eldritch2::EqualTo<Eldritch2::AbstractString<Character, Allocator>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref EqualTo instance.
	ETConstexpr EqualTo(const EqualTo&) ETNoexceptHint = default;
	//!	Constructs this @ref EqualTo instance.
	ETConstexpr EqualTo() ETNoexceptHint = default;

	~EqualTo() = default;

	// ---------------------------------------------------

public:
	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator()(const Eldritch2::AbstractString<Character, Allocator>& lhs, const Eldritch2::AbstractString<Character, Allocator>& rhs) const ETNoexceptHint {
		return lhs == rhs;
	}

	ETInlineHint ETForceInlineHint ETPureFunctionHint bool operator()(const Eldritch2::AbstractString<Character, Allocator>& lhs, AbstractStringSpan<Character> rhs) const ETNoexceptHint {
		return lhs == rhs;
	}
};

template <typename Character, class Allocator>
class Eldritch2::Hash<Eldritch2::AbstractString<Character, Allocator>> : public Eldritch2::Hash<Eldritch2::AbstractStringSpan<Character>> {};

template <typename Character, typename Allocator>
struct fmt::is_contiguous<eastl::basic_string<Character, Allocator>> : public eastl::true_type {};

template <typename Character, class Allocator, typename OutCharacter>
struct fmt::formatter<Eldritch2::AbstractString<Character, Allocator>, OutCharacter> : public fmt::formatter<Eldritch2::AbstractStringSpan<Character>, OutCharacter> {};
