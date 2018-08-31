/*==================================================================*\
  Memory.Win32.inl
  ------------------------------------------------------------------
  Purpose:
  Utility functions that serve as replacements for standard C bulk
  memory operations and may be faster (but are guaranteed to be
  equivalent to the compiler's at worst) than their ANSI equivalent.

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//
#include <cstdarg>
#include <cstring>
#include <cwctype>
#include <cctype>
#include <cstdio>
#include <cwchar>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETForceInlineHint ETPureFunctionHint void* CopyMemory(void* ETRestrictPtrHint destination, const void* ETRestrictPtrHint source, size_t lengthInBytes) ETNoexceptHint {
	return std::memcpy(destination, source, lengthInBytes);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint void* CopyMemoryNonTemporal(void* ETRestrictPtrHint destination, const void* ETRestrictPtrHint source, size_t lengthInBytes) ETNoexceptHint {
	return std::memcpy(destination, source, lengthInBytes);
}

// ---------------------------------------------------

template <typename T>
ETForceInlineHint ETPureFunctionHint T* CopyArray(T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, size_t sizeInElements) ETNoexceptHint {
	return static_cast<T*>(Eldritch2::CopyMemory(destination, source, sizeInElements * sizeof(T)));
}

// ---------------------------------------------------

template <typename T, size_t sizeInElements>
ETForceInlineHint ETPureFunctionHint auto CopyArray(T (&destination)[sizeInElements], const T (&source)[sizeInElements]) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::CopyArray(destination, source, sizeInElements), destination);
}

// ---------------------------------------------------

template <typename T>
ETForceInlineHint ETPureFunctionHint T* CopyArrayNonTemporal(T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, size_t sizeInElements) ETNoexceptHint {
	return static_cast<T*>(Eldritch2::CopyMemoryNonTemporal(destination, source, sizeInElements * sizeof(T)));
}

// ---------------------------------------------------

template <typename T, size_t size>
ETForceInlineHint ETPureFunctionHint auto CopyArrayNonTemporal(T (&destination)[size], const T (&source)[size]) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::CopyArrayNonTemporal(destination, source, size), destination);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint void* MoveMemory(void* destination, const void* source, size_t lengthInBytes) ETNoexceptHint {
	return std::memmove(destination, source, lengthInBytes);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint void* MoveMemoryNonTemporal(void* destination, const void* source, size_t lengthInBytes) ETNoexceptHint {
	return Eldritch2::MoveMemory(destination, source, lengthInBytes);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint void* SetMemory(void* destination, int bitPattern, size_t lengthInBytes) ETNoexceptHint {
	return std::memset(destination, bitPattern, lengthInBytes);
}

// ---------------------------------------------------

template <typename T>
ETForceInlineHint ETPureFunctionHint T* ZeroMemory(T* destination, size_t sizeInElements) ETNoexceptHint {
	static_assert(eastl::is_trivially_copyable<T>::value, "Zeroing polymorphic types destroys virtual function tables and is disabled.");

	// ---

	return static_cast<T*>(Eldritch2::SetMemory(destination, 0, sizeInElements * sizeof(T)));
}

// ---------------------------------------------------

//	Semi-hack: Define a specialization for the void type for compatibility with the more conventional usage
ETForceInlineHint ETPureFunctionHint void* ZeroMemory(void* destination, size_t bufferSizeInBytes) ETNoexceptHint {
	return Eldritch2::SetMemory(destination, 0, bufferSizeInBytes);
}

// ---------------------------------------------------

template <typename T, size_t size>
ETForceInlineHint ETPureFunctionHint auto ZeroMemory(T (&destination)[size]) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::ZeroMemory(static_cast<T*>(destination), size), destination);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderBuffersCaseInsensitive(const void* lhs, const void* rhs, size_t spanInBytes) ETNoexceptHint {
	return _memicmp(lhs, rhs, spanInBytes);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderBuffers(const void* lhs, const void* rhs, size_t spanInBytes) ETNoexceptHint {
	return std::memcmp(lhs, rhs, spanInBytes);
}

// ---------------------------------------------------

template <typename T>
ETForceInlineHint ETPureFunctionHint int OrderArrays(const T* lhs, const T* rhs, size_t sizeInElements) ETNoexceptHint {
	return Eldritch2::CompareMemory(lhs, rhs, sizeInElements * sizeof(T));
}

// ---------------------------------------------------

template <typename T, size_t size>
ETForceInlineHint ETPureFunctionHint int OrderArrays(const T (&lhs)[size], const T (&rhs)[size]) ETNoexceptHint {
	return Eldritch2::CompareArray(static_cast<const T*>(lhs), static_cast<const T*>(rhs), size);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStrings(const char* lhs, const char* rhs) ETNoexceptHint {
	return std::strcmp(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStrings(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint {
	return std::wcscmp(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStrings(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return std::strncmp(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStrings(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return std::wcsncmp(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint int OrderStrings(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::OrderStrings(static_cast<const char*>(lhs), static_cast<const char*>(rhs), lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint int OrderStrings(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::OrderStrings(static_cast<const wchar_t*>(lhs), static_cast<const wchar_t*>(rhs), length);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive(const char* lhs, const char* rhs) ETNoexceptHint {
	return _stricmp(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint {
	return _wcsicmp(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return _strnicmp(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return _wcsnicmp(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint {
	return std::_memicmp(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::OrderStringsCaseInsensitive(static_cast<const wchar_t*>(lhs), static_cast<const wchar_t*>(rhs));
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqual(const char* lhs, const char* rhs) ETNoexceptHint {
	return 0 == Eldritch2::OrderStrings(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqual(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint {
	return 0 == Eldritch2::OrderStrings(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqual(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return 0 == Eldritch2::OrderStrings(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqual(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return 0 == Eldritch2::OrderStrings(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint bool StringsEqual(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::StringsEqual(static_cast<const char*>(lhs), static_cast<const char*>(rhs), lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint bool StringsEqual(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::StringsEqual(static_cast<const wchar_t*>(lhs), static_cast<const wchar_t*>(rhs), lengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive(const char* lhs, const char* rhs) ETNoexceptHint {
	return 0 == Eldritch2::OrderStringsCaseInsensitive(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint {
	return 0 == Eldritch2::OrderStringsCaseInsensitive(lhs, rhs);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return 0 == Eldritch2::OrderStringsCaseInsensitive(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint {
	return 0 == Eldritch2::OrderStringsCaseInsensitive(lhs, rhs, lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::StringsEqualCaseInsensitive(static_cast<const char*>(lhs), static_cast<const char*>(rhs), lengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::StringsEqualCaseInsensitive(static_cast<const wchar_t*>(lhs), static_cast<const wchar_t*>(rhs), length);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint size_t StringLength(const char* string) ETNoexceptHint {
	return std::strlen(string);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint size_t StringLength(const wchar_t* string) ETNoexceptHint {
	return std::wcslen(string);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint size_t StringLength(const char (&string)[length]) ETNoexceptHint {
	return Eldritch2::StringLength(static_cast<const char*>(string));
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint size_t StringLength(const wchar_t (&string)[length]) ETNoexceptHint {
	return Eldritch2::StringLength(static_cast<const wchar_t*>(string));
}

// ---------------------------------------------------

ET_PUSH_MSVC_WARNING_STATE(disable : 4996)
ETForceInlineHint ETPureFunctionHint char* CopyString(char* destination, const char* source, size_t maxLengthInCharacters) ETNoexceptHint {
	return std::strncpy(destination, source, maxLengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* CopyString(wchar_t* destination, const wchar_t* source, size_t maxLengthInCharacters) ETNoexceptHint {
	return std::wcsncpy(destination, source, maxLengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint auto CopyString(char (&destination)[length], const char* source) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::CopyString(static_cast<char*>(destination), source, length), destination);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint auto CopyString(wchar_t (&destination)[length], const wchar_t* source) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::CopyString(static_cast<wchar_t*>(destination), source, length), destination);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint char* AppendString(char* destination, const char* source, size_t maxLengthInCharacters) ETNoexceptHint {
	return std::strncat(destination, source, maxLengthInCharacters);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* AppendString(wchar_t* destination, const wchar_t* source, size_t maxLengthInCharacters) ETNoexceptHint {
	return std::wcsncat(destination, source, maxLengthInCharacters);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint auto AppendString(char (&destination)[length], const char* source) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::AppendString(static_cast<char*>(destination), source, length), destination);
}

// ---------------------------------------------------

template <size_t length>
ETForceInlineHint ETPureFunctionHint auto AppendString(wchar_t (&destination)[length], const wchar_t* source) ETNoexceptHint -> decltype(destination) {
	return (Eldritch2::AppendString(static_cast<wchar_t*>(destination), source, length), destination);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint char* Find(char* string, const char character) ETNoexceptHint {
	return std::strchr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* Find(const char* string, const char character) ETNoexceptHint {
	return std::strchr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint char* Find(char* string, const char* substring) ETNoexceptHint {
	return std::strstr(string, substring);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* Find(const char* string, const char* substring) ETNoexceptHint {
	return std::strstr(string, substring);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* Find(wchar_t* string, const wchar_t character) ETNoexceptHint {
	return std::wcschr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* Find(const wchar_t* string, const wchar_t character) ETNoexceptHint {
	return std::wcschr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* Find(wchar_t* string, const wchar_t* substring) ETNoexceptHint {
	return std::wcsstr(string, substring);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* Find(const wchar_t* string, const wchar_t* substring) ETNoexceptHint {
	return std::wcsstr(string, substring);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint char* FindLast(char* string, const char character) ETNoexceptHint {
	return std::strrchr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* FindLast(const char* string, const char character) ETNoexceptHint {
	return std::strrchr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint char* FindLast(char* string, const char* substring) ETNoexceptHint {
	return std::find_end(string, FindTerminator(string), substring, FindTerminator(substring));
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* FindLast(const char* string, const char* substring) ETNoexceptHint {
	return std::find_end(string, FindTerminator(string), substring, FindTerminator(substring));
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* FindLast(wchar_t* string, const wchar_t character) ETNoexceptHint {
	return std::wcsrchr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* FindLast(const wchar_t* string, const wchar_t character) ETNoexceptHint {
	return std::wcsrchr(string, character);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* FindLast(wchar_t* string, const wchar_t* substring) ETNoexceptHint {
	return std::find_end(string, FindTerminator(string), substring, FindTerminator(substring));
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* FindLast(const wchar_t* string, const wchar_t* substring) ETNoexceptHint {
	return std::find_end(string, FindTerminator(string), substring, FindTerminator(substring));
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint char* FindTerminator(char* string) ETNoexceptHint {
	return string + Eldritch2::StringLength(string);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* FindTerminator(const char* string) ETNoexceptHint {
	return string + Eldritch2::StringLength(string);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint wchar_t* FindTerminator(wchar_t* string) ETNoexceptHint {
	return string + Eldritch2::StringLength(string);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* FindTerminator(const wchar_t* string) ETNoexceptHint {
	return string + Eldritch2::StringLength(string);
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* TrimLeadingWhitespace(const char* begin, const char* const end) ETNoexceptHint {
	return std::find_if(begin, end, [](char character) { return std::isspace(int(character)) != 0; });
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* TrimLeadingWhitespace(const wchar_t* begin, const wchar_t* const end) ETNoexceptHint {
	return std::find_if(begin, end, [](wchar_t character) { return std::iswspace(::wint_t(character)) != 0; });
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const char* TrimTrailingWhitespace(const char* const begin, const char* end) ETNoexceptHint {
	return std::find_if(std::make_reverse_iterator(end), std::make_reverse_iterator(begin), [](char character) { return std::isspace(int(character)) != 0; }).base();
}

// ---------------------------------------------------

ETForceInlineHint ETPureFunctionHint const wchar_t* TrimTrailingWhitespace(const wchar_t* const begin, const wchar_t* end) ETNoexceptHint {
	return std::find_if(std::make_reverse_iterator(end), std::make_reverse_iterator(begin), [](wchar_t character) { return std::iswspace(::wint_t(character)) != 0; }).base();
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint const wchar_t* EmptyForNull(const wchar_t* string) ETNoexceptHint {
	return string ? string : L"";
}

// ---------------------------------------------------

ETConstexpr ETForceInlineHint ETPureFunctionHint const char* EmptyForNull(const char* string) ETNoexceptHint {
	return string ? string : "";
}

// ---------------------------------------------------

template <typename T>
ETForceInlineHint ETPureFunctionHint const T* FindArrayDifference(const T* ETRestrictPtrHint lhs, const T* ETRestrictPtrHint rhs, size_t sizeInElements) ETNoexceptHint {
	return std::mismatch(lhs, lhs + sizeInElements, rhs, rhs + sizeInElements).first;
}

// ---------------------------------------------------

template <typename T>
ETForceInlineHint ETPureFunctionHint T* FindArrayDifference(T* ETRestrictPtrHint lhs, T* ETRestrictPtrHint rhs, size_t sizeInElements) ETNoexceptHint {
	return std::mismatch(lhs, lhs + sizeInElements, rhs, rhs + sizeInElements).first;
}

// ---------------------------------------------------

template <typename T, size_t length>
ETForceInlineHint ETPureFunctionHint const T* FindArrayDifference(const T (&lhs)[length], const T (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::FindArrayDifference(static_cast<const T*>(lhs), static_cast<const T*>(rhs), length);
}

// ---------------------------------------------------

template <typename T, size_t length>
ETForceInlineHint ETPureFunctionHint T* FindArrayDifference(T (&lhs)[length], T (&rhs)[length]) ETNoexceptHint {
	return Eldritch2::FindArrayDifference(static_cast<T*>(lhs), static_cast<T*>(rhs), length);
}

} // namespace Eldritch2
