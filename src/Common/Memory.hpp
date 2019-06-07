/*==================================================================*\
  Memory.hpp
  ------------------------------------------------------------------
  Purpose:
  Utility functions that serve as replacements for standard C bulk
  memory operations and may be faster (but are guaranteed to be
  equivalent to the compiler's at worst) than their ANSI equivalent.

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

//	Performs a raw, bitwise comparison of the first span bytes pointed to by lhs against the first span bytes pointed to
//	by rhs. If all bytes are equal, the result is 0. Otherwise, the result is greater than zero if the first byte that does
//	not match in lhs is greater than the first byte that does not match in rhs and vice versa in the less-than case.
ETPureFunctionHint int OrderBuffersCaseInsensitive(const void* lhs, const void* rhs, size_t spanInBytes) ETNoexceptHint;

//	Performs a raw, bitwise comparison of the first span bytes pointed to by lhs against the first span bytes pointed to
//	by rhs. If all bytes are equal, the result is 0. Otherwise, the result is greater than zero if the first byte that does
//	not match in lhs is greater than the first byte that does not match in rhs and vice versa in the less-than case.
ETPureFunctionHint int OrderBuffers(const void* lhs, const void* rhs, size_t spanInBytes) ETNoexceptHint;

//!	Utility wrapper around @ref OrderBuffers() that handles casting and automatically adjusts for element size.
template <typename T, size_t length>
ETPureFunctionHint int OrderArrays(const T (&lhs)[length], const T (&rhs)[length]) ETNoexceptHint;
//!	Utility wrapper around @ref OrderBuffers() that handles casting and automatically adjusts for element size.
template <typename T>
ETPureFunctionHint int OrderArrays(const T* lhs, const T* rhs, size_t sizeInElements) ETNoexceptHint;

//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case.
template <size_t length>
ETPureFunctionHint int OrderStrings(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case.
template <size_t length>
ETPureFunctionHint int OrderStrings(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case.
ETPureFunctionHint int OrderStrings(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case.
ETPureFunctionHint int OrderStrings(const char* lhs, const char* rhs) ETNoexceptHint;

/*	Performs a case-insensitive comparison of the individual characters in lhs against the individual characters in rhs.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case. */
ETPureFunctionHint int OrderStringsCaseInsensitive(const char* lhs, const char* rhs) ETNoexceptHint;
/*	Performs a case-insensitive comparison of the individual characters in lhs against the individual characters in rhs.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case. */
ETPureFunctionHint int OrderStringsCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint;
/*	Performs a case-insensitive comparison of the individual characters in lhs against the individual characters in rhs.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case. */
ETPureFunctionHint int OrderStringsCaseInsensitive(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint;
/*	Performs a case-insensitive comparison of the individual characters in lhs against the individual characters in rhs.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case. */
ETPureFunctionHint int OrderStringsCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint;
/*	Performs a case-insensitive comparison of the individual characters in lhs against the individual characters in rhs.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case. */
template <size_t length>
ETPureFunctionHint int OrderStringsCaseInsensitive(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint;
/*	Performs a case-insensitive comparison of the individual characters in lhs against the individual characters in rhs.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in lhs is greater than the first character that does not match in rhs and vice versa in the less-than case. */
template <size_t length>
ETPureFunctionHint int OrderStringsCaseInsensitive(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint;

//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is true, otherwise the function returns false.
template <size_t length>
ETPureFunctionHint bool StringsEqual(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is true, otherwise the function returns false.
template <size_t length>
ETPureFunctionHint bool StringsEqual(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is true, otherwise the function returns false.
ETPureFunctionHint bool StringsEqual(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is true, otherwise the function returns false.
ETPureFunctionHint bool StringsEqual(const char* lhs, const char* rhs) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is true, otherwise the function returns false.
ETPureFunctionHint bool StringsEqual(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint;
//	Performs a raw, bitwise comparison of the individual characters in lhs against the individual characters in rhs.
//	If all characters are equal, the result is true, otherwise the function returns false.
ETPureFunctionHint bool StringsEqual(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint;

ETPureFunctionHint bool StringsEqualCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs, size_t lengthInCharacters) ETNoexceptHint;
ETPureFunctionHint bool StringsEqualCaseInsensitive(const char* lhs, const char* rhs, size_t lengthInCharacters) ETNoexceptHint;
ETPureFunctionHint bool StringsEqualCaseInsensitive(const wchar_t* lhs, const wchar_t* rhs) ETNoexceptHint;
ETPureFunctionHint bool StringsEqualCaseInsensitive(const char* lhs, const char* rhs) ETNoexceptHint;

template <size_t length>
ETPureFunctionHint bool StringsEqualCaseInsensitive(const wchar_t (&lhs)[length], const wchar_t (&rhs)[length]) ETNoexceptHint;
template <size_t length>
ETPureFunctionHint bool StringsEqualCaseInsensitive(const char (&lhs)[length], const char (&rhs)[length]) ETNoexceptHint;

// ---------------------------------------------------

ETConstexpr ETPureFunctionHint size_t GetHashCode(const void* pointer, size_t seed = 0u) ETNoexceptHint;

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t sizeInBytes, size_t seed) ETNoexceptHint;

// ---------------------------------------------------

//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
ETCpp14Constexpr ETPureFunctionHint size_t StringLength(const wchar_t* string) ETNoexceptHint;
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
ETCpp14Constexpr ETPureFunctionHint size_t StringLength(const char* string) ETNoexceptHint;

// ---------------------------------------------------

//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint const char* Find(const char* string, const char character) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint char* Find(char* string, const char character) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
ETPureFunctionHint const char* Find(const char* string, const char* substring) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
ETPureFunctionHint char* Find(char* string, const char* substring) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint const wchar_t* Find(const wchar_t* string, const wchar_t character) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint wchar_t* Find(wchar_t* string, const wchar_t character) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint const wchar_t* Find(const wchar_t* string, const wchar_t* substring) ETNoexceptHint;
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint wchar_t* Find(wchar_t* string, const wchar_t* substring) ETNoexceptHint;

//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint const char* FindLast(const char* string, const char character) ETNoexceptHint;
//	Locates/returns a pointer to the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint char* FindLast(char* string, const char character) ETNoexceptHint;
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
ETPureFunctionHint const char* FindLast(const char* string, const char* substring) ETNoexceptHint;
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
ETPureFunctionHint char* FindLast(char* string, const char* substring) ETNoexceptHint;
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint const wchar_t* FindLast(const wchar_t* string, const wchar_t character) ETNoexceptHint;
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint wchar_t* FindLast(wchar_t* string, const wchar_t character) ETNoexceptHint;
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint const wchar_t* FindLast(const wchar_t* string, const wchar_t* substring) ETNoexceptHint;
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
ETPureFunctionHint wchar_t* FindLast(wchar_t* string, const wchar_t* substring) ETNoexceptHint;

/*!	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
 *	an unterminated string may result in a buffer read overrun. */
ETPureFunctionHint const wchar_t* FindTerminator(const wchar_t* string) ETNoexceptHint;
/*!	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
 *	an unterminated string may result in a buffer read overrun. */
ETPureFunctionHint wchar_t* FindTerminator(wchar_t* string) ETNoexceptHint;
/*!	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
 *	an unterminated string may result in a buffer read overrun. */
ETPureFunctionHint const char* FindTerminator(const char* string) ETNoexceptHint;
/*!	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
 *	an unterminated string may result in a buffer read overrun. */
ETPureFunctionHint char* FindTerminator(char* string) ETNoexceptHint;

ETPureFunctionHint const wchar_t* TrimLeadingWhitespace(const wchar_t* begin, const wchar_t* const end) ETNoexceptHint;
ETPureFunctionHint const char*    TrimLeadingWhitespace(const char* begin, const char* const end) ETNoexceptHint;

ETPureFunctionHint const wchar_t* TrimTrailingWhitespace(const wchar_t* begin, const wchar_t* const end) ETNoexceptHint;
ETPureFunctionHint const char*    TrimTrailingWhitespace(const char* begin, const char* const end) ETNoexceptHint;

ETConstexpr ETPureFunctionHint const wchar_t* EmptyForNull(const wchar_t* string) ETNoexceptHint;
ETConstexpr ETPureFunctionHint const char* EmptyForNull(const char* string) ETNoexceptHint;

// ---------------------------------------------------

/*!	Attempts to locate a pointer to the first 'different' element in lhs as compared to rhs.
 *	If the arrays are identical, a null pointer is returned instead. */
template <typename T>
ETPureFunctionHint const T* FindArrayDifference(const T* ETRestrictPtrHint lhs, const T* ETRestrictPtrHint rhs, size_t sizeInElements) ETNoexceptHint;
/*!	Attempts to locate a pointer to the first 'different' element in lhs as compared to rhs.
 *	If the arrays are identical, a null pointer is returned instead. */
template <typename T>
ETPureFunctionHint T* FindArrayDifference(T* ETRestrictPtrHint lhs, T* ETRestrictPtrHint rhs, size_t sizeInElements) ETNoexceptHint;
/*!	Attempts to locate a pointer to the first 'different' element in lhs as compared to rhs.
 *	If the arrays are identical, a null pointer is returned instead. */
template <typename T, size_t length>
ETPureFunctionHint const T* FindArrayDifference(const T (&lhs)[length], const T (&rhs)[length]) ETNoexceptHint;
/*!	Attempts to locate a pointer to the first 'different' element in lhs as compared to rhs.
 *	If the arrays are identical, a null pointer is returned instead. */
template <typename T, size_t length>
ETPureFunctionHint T* FindArrayDifference(T (&lhs)[length], T (&rhs)[length]) ETNoexceptHint;

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#if ET_PLATFORM_WINDOWS
#	include <Common/Memory.Win32.inl>
#elif ET_PLATFORM_MAC
#	include <Common/Memory.Mac.inl>
#elif ET_PLATFORM_LINUX
#	include <Common/Memory.Linux.inl>
#endif
#include <Common/Memory.inl>
//------------------------------------------------------------------//
