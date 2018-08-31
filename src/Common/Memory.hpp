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

// Thanks for polluting the global namespace with your macros, Microsoft!
#if defined(CopyMemory)
#	pragma push_macro("CopyMemory")
#	undef CopyMemory
#	define POP_COPYMEMORY
#endif
#if defined(MoveMemory)
#	pragma push_macro("MoveMemory")
#	undef MoveMemory
#	define POP_MOVEMEMORY
#endif
#if defined(SetMemory)
#	pragma push_macro("SetMemory")
#	undef SetMemory
#	define POP_SETMEMORY
#endif
#if defined(ZeroMemory)
#	pragma push_macro("ZeroMemory")
#	undef ZeroMemory
#	define POP_ZEROMEMORY
#endif

namespace Eldritch2 {

//! Copies a span of bytes.
/*!	@param[in] destination Pointer to start copying memory to.
	@param[in] source Pointer to start copying memory from.
	@param[in] lengthInBytes Number of bytes to copy from _sourceBuffer_ to _destinationBuffer_.
	@pre The regions defined by [_destinationBuffer_, _destinationBuffer_ + _lengthInBytes_), [_sourceBuffer_, _sourceBuffer_ + _lengthInBytes_) should not overlap; if this is a possibility use MoveMemory() instead.
	@see CopyMemoryNonTemporal()
	@see MoveMemory() */
ETPureFunctionHint void* CopyMemory(void* ETRestrictPtrHint destination, const void* ETRestrictPtrHint source, size_t lengthInBytes) ETNoexceptHint;

//! Copies a span of bytes.
/*! @param[in] destination Pointer to start copying memory to.
	@param[in] source Pointer to start copying memory from.
	@param[in] lengthInBytes Number of bytes to copy from _sourceBuffer_ to _destinationBuffer_.
	@remark Uses non-temporal (unlikely to be accessed again soon) cache hinting semantics.
	@pre The regions defined by [_destinationBuffer_, _destinationBuffer_ + _lengthInBytes_), [_sourceBuffer_, _sourceBuffer_ + _lengthInBytes_) should not overlap; if this is a possibility use MoveMemoryNonTemporal() instead.
	@see CopyMemory()
	@see MoveMemoryNonTemporal() */
ETPureFunctionHint void* CopyMemoryNonTemporal(void* ETRestrictPtrHint destination, const void* ETRestrictPtrHint source, size_t lengthInBytes) ETNoexceptHint;

//!	Utility wrapper around @ref CopyMemory() that handles casting and automatically adjusts for element size.
template <typename T>
ETPureFunctionHint T* CopyArray(T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, size_t sizeInElements) ETNoexceptHint;
//!	Utility wrapper around @ref CopyMemory() that handles casting and automatically adjusts for element size.
template <typename T, size_t length>
ETPureFunctionHint auto CopyArray(T (&destination)[length], const T (&source)[length]) ETNoexceptHint -> decltype(destination);

//!	Utility wrapper around @ref CopyMemoryNonTemporal that handles casting and automatically adjusts for element size.
template <typename T>
ETPureFunctionHint T* CopyArrayNonTemporal(T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, size_t sizeInElements) ETNoexceptHint;
//!	Utility wrapper around @ref CopyMemoryNonTemporal that handles casting and automatically adjusts for element size.
template <typename T, size_t length>
ETPureFunctionHint auto CopyArrayNonTemporal(T (&destination)[length], const T (&source)[length]) ETNoexceptHint -> decltype(destination);

// ---------------------------------------------------

//!	Copies bytes from the address specified by source to the location specified by destination.
/*!	Unlike @ref CopyMemory(), the source and destination buffers may overlap. */
ETPureFunctionHint void* MoveMemory(void* destination, const void* source, size_t lengthInBytes) ETNoexceptHint;

//!	Copies bytes from the address specified by source to the location specified by destination with non-temporal (unlikely to be accessed again soon) semantics.
/*!	As with @ref MoveMemory() and unlike @ref CopyMemory(), the source and destination arrays may overlap. */
ETPureFunctionHint void* MoveMemoryNonTemporal(void* destination, const void* source, size_t lengthInBytes) ETNoexceptHint;

// ---------------------------------------------------

//!	Sets the value of length bytes in the array specified by destination to the bit pattern denoted in bitPattern.
ETPureFunctionHint void* SetMemory(void* destination, const int bitPattern, size_t lengthInBytes) ETNoexceptHint;

//!	Zeroes out the entirety of sizeInElements instances of type T.
template <typename T>
ETPureFunctionHint T* ZeroMemory(T* destination, size_t sizeInElements) ETNoexceptHint;

//!	Zeroes out the entirety of length instances of type T.
template <typename T, size_t length>
ETPureFunctionHint auto ZeroMemory(T (&destination)[length]) ETNoexceptHint -> decltype(destination);

// ---------------------------------------------------

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

ETCpp14Constexpr ETPureFunctionHint size_t HashMemory(const void* memory, size_t sizeInBytes, size_t seed) ETNoexceptHint;

// ---------------------------------------------------

//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
template <size_t lengthInCharacters>
ETPureFunctionHint size_t StringLength(const wchar_t (&str)[lengthInCharacters]) ETNoexceptHint;
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
template <size_t lengthInCharacters>
ETPureFunctionHint size_t StringLength(const char (&str)[lengthInCharacters]) ETNoexceptHint;
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
ETPureFunctionHint size_t StringLength(const wchar_t* string) ETNoexceptHint;
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
ETPureFunctionHint size_t StringLength(const char* string) ETNoexceptHint;

// ---------------------------------------------------

//!	Copies the C string specified by source into the array pointed to by destination, including the terminator.
template <size_t lengthInCharacters>
ETPureFunctionHint auto CopyString(wchar_t (&destination)[lengthInCharacters], const wchar_t* source) ETNoexceptHint -> decltype(destination);
//!	Copies the C string specified by source into the array pointed to by destination, including the terminator.
template <size_t lengthInCharacters>
ETPureFunctionHint auto CopyString(char (&destination)[lengthInCharacters], const char* source) ETNoexceptHint -> decltype(destination);
//!	Copies the C string specified by source into the array pointed to by destination, including the terminator.
ETPureFunctionHint wchar_t* CopyString(wchar_t* destination, const wchar_t* source, size_t maxLengthInCharacters) ETNoexceptHint;
//!	Copies the C string specified by source into the array pointed to by destination, including the terminator.
ETPureFunctionHint char* CopyString(char* destination, const char* source, size_t maxLengthInCharacters) ETNoexceptHint;

// ---------------------------------------------------

//! Appends the C string specified by source after the C string denoted by destination.
template <size_t lengthInCharacters>
ETPureFunctionHint auto AppendString(wchar_t (&destination)[lengthInCharacters], const wchar_t* source) ETNoexceptHint -> decltype(destination);
//!	Appends the C string specified by source after the C string denoted by destination.
template <size_t lengthInCharacters>
ETPureFunctionHint auto AppendString(char (&destination)[lengthInCharacters], const char* source) ETNoexceptHint -> decltype(destination);
//!	Appends the C string specified by source after the C string denoted by destination.
ETPureFunctionHint wchar_t* AppendString(wchar_t* destination, const wchar_t* source, size_t maxLengthInCharacters) ETNoexceptHint;
//!	Appends the C string specified by source after the C string denoted by destination.
ETPureFunctionHint char* AppendString(char* destination, const char* source, size_t maxLengthInCharacters) ETNoexceptHint;

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

#if defined(POP_COPYMEMORY)
#	pragma pop_macro("CopyMemory")
#	undef POP_COPYMEMORY
#endif
#if defined(POP_MOVEMEMORY)
#	pragma pop_macro("MoveMemory")
#	undef POP_MOVEMEMORY
#endif
#if defined(POP_SETMEMORY)
#	pragma pop_macro("SetMemory")
#	undef POP_SETMEMORY
#endif
#if defined(POP_ZEROMEMORY)
#	pragma pop_macro("ZeroMemory")
#	undef POP_ZEROMEMORY
#endif
