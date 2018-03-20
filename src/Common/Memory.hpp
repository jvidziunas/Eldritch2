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
#if defined( CopyMemory )
#	undef CopyMemory
#endif
#if defined( MoveMemory )
#	undef MoveMemory
#endif
#if defined( SetMemory )
#	undef SetMemory
#endif
#if defined( ZeroMemory )
#	undef ZeroMemory
#endif

namespace Eldritch2 {

//! Copies a span of bytes.
/*!	@param[in] destinationBuffer Pointer to start copying memory to.
	@param[in] sourceBuffer Pointer to start copying memory from.
	@param[in] lengthInBytes Number of bytes to copy from _sourceBuffer_ to _destinationBuffer_.
	@pre The regions defined by [_destinationBuffer_, _destinationBuffer_ + _lengthInBytes_), [_sourceBuffer_, _sourceBuffer_ + _lengthInBytes_) should not overlap; if this is a possibility use MoveMemory() instead.
	@see CopyMemoryNonTemporal()
	@see MoveMemory() */
	ETPureFunctionHint void*	CopyMemory( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, size_t lengthInBytes );

//! Copies a span of bytes.
/*! @param[in] destinationBuffer Pointer to start copying memory to.
	@param[in] sourceBuffer Pointer to start copying memory from.
	@param[in] lengthInBytes Number of bytes to copy from _sourceBuffer_ to _destinationBuffer_.
	@remark Uses non-temporal (unlikely to be accessed again soon) cache hinting semantics.
	@pre The regions defined by [_destinationBuffer_, _destinationBuffer_ + _lengthInBytes_), [_sourceBuffer_, _sourceBuffer_ + _lengthInBytes_) should not overlap; if this is a possibility use MoveMemoryNonTemporal() instead.
	@see CopyMemory()
	@see MoveMemoryNonTemporal() */
	ETPureFunctionHint void*	CopyMemoryNonTemporal( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, size_t lengthInBytes );

//!	Utility wrapper around @ref CopyMemory() that handles casting and automatically adjusts for element size.
	template <typename T>
	ETPureFunctionHint T*		CopyArray( T* ETRestrictPtrHint destinationArray, const T* ETRestrictPtrHint sourceArray, size_t sizeInElements );
//!	Utility wrapper around @ref CopyMemory() that handles casting and automatically adjusts for element size.
	template <typename T, size_t sizeInElements>
	ETPureFunctionHint auto		CopyArray( T(&destinationArray)[sizeInElements], const T(&sourceArray)[sizeInElements] ) -> decltype(destinationArray);

//!	Utility wrapper around @ref CopyMemoryNonTemporal that handles casting and automatically adjusts for element size.
	template <typename T>
	ETPureFunctionHint T*		CopyArrayNonTemporal( T* ETRestrictPtrHint destinationArray, const T* ETRestrictPtrHint sourceArray, size_t sizeInElements );
//!	Utility wrapper around @ref CopyMemoryNonTemporal that handles casting and automatically adjusts for element size.
	template <typename T, size_t sizeInElements>
	ETPureFunctionHint auto		CopyArrayNonTemporal( T(&destinationArray)[sizeInElements], const T(&sourceArray)[sizeInElements] ) -> decltype(destinationArray);

// ---------------------------------------------------

//!	Copies bytes from the address specified by source to the location specified by destination.
/*!	Unlike @ref CopyMemory(), the source and destination buffers may overlap. */
	ETPureFunctionHint void*	MoveMemory( void* destinationBuffer, const void* sourceBuffer, size_t lengthInBytes );

//!	Copies bytes from the address specified by source to the location specified by destination with non-temporal (unlikely to be accessed again soon) semantics.
/*!	As with @ref MoveMemory() and unlike @ref CopyMemory(), the source and destination arrays may overlap. */
	ETPureFunctionHint void*	MoveMemoryNonTemporal( void* destinationBuffer, const void* sourceBuffer, size_t lengthInBytes );

// ---------------------------------------------------

//!	Sets the value of length bytes in the array specified by destination to the bit pattern denoted in bitPattern.
	ETPureFunctionHint void*	SetMemory( void* destinationBuffer, const int bitPattern, size_t lengthInBytes );

//!	Zeroes out the entirety of numElements instances of type T.
	template <typename T>
	ETPureFunctionHint T*		ZeroMemory( T* destinationArray, size_t sizeInElements );

//!	Zeroes out the entirety of numElements instances of type T.
	template <typename T, size_t sizeInElements>
	ETPureFunctionHint auto		ZeroMemory( T (&destinationArray)[sizeInElements] ) -> decltype(destinationArray);

// ---------------------------------------------------

//	Performs a raw, bitwise comparison of the first span bytes pointed to by buffer0 against the first span bytes pointed to
//	by buffer1. If all bytes are equal, the result is 0. Otherwise, the result is greater than zero if the first byte that does
//	not match in buffer0 is greater than the first byte that does not match in buffer1 and vice versa in the less-than case.
	ETPureFunctionHint int	OrderBuffersCaseInsensitive( const void* buffer0, const void* buffer1, size_t spanInBytes );

//	Performs a raw, bitwise comparison of the first span bytes pointed to by buffer0 against the first span bytes pointed to
//	by buffer1. If all bytes are equal, the result is 0. Otherwise, the result is greater than zero if the first byte that does
//	not match in buffer0 is greater than the first byte that does not match in buffer1 and vice versa in the less-than case.
	ETPureFunctionHint int	OrderBuffers( const void* buffer0, const void* buffer1, size_t spanInBytes );

//!	Utility wrapper around @ref OrderBuffers() that handles casting and automatically adjusts for element size.
	template <typename T, size_t sizeInElements>
	ETPureFunctionHint int	OrderArrays( const T (&array0)[sizeInElements], const T (&array1)[sizeInElements] );
//!	Utility wrapper around @ref OrderBuffers() that handles casting and automatically adjusts for element size.
	template <typename T>
	ETPureFunctionHint int	OrderArrays( const T* array0, const T* array1, size_t sizeInElements );

//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	template <size_t lengthInCharacters>
	ETPureFunctionHint int	OrderStrings( const wchar_t (&string0)[lengthInCharacters], const wchar_t (&string1)[lengthInCharacters] );
//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	template <size_t lengthInCharacters>
	ETPureFunctionHint int	OrderStrings( const char (&string0)[lengthInCharacters], const char (&string1)[lengthInCharacters] );
//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	ETPureFunctionHint int	OrderStrings( const wchar_t* string0, const wchar_t* string1 );
//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	ETPureFunctionHint int	OrderStrings( const char* string0, const char* string1 );

/*	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case. */
	ETPureFunctionHint int	OrderStringsCaseInsensitive( const char* str0, const char* str1 );
/*	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case. */
	ETPureFunctionHint int	OrderStringsCaseInsensitive( const wchar_t* str0, const wchar_t* str1 );
/*	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case. */
	ETPureFunctionHint int	OrderStringsCaseInsensitive( const char* str0, const char* str1, size_t stringLengthInCharacters );
/*	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case. */
	ETPureFunctionHint int	OrderStringsCaseInsensitive( const wchar_t* str0, const wchar_t* str1, size_t stringLengthInCharacters );
/*	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case. */
	template <size_t lengthInCharacters>
	ETPureFunctionHint int	OrderStringsCaseInsensitive( const char (&str0)[lengthInCharacters], const char (&str1)[lengthInCharacters] );
/*	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
 *	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
 *	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case. */
	template <size_t lengthInCharacters>
	ETPureFunctionHint int	OrderStringsCaseInsensitive( const wchar_t (&str0)[lengthInCharacters], const wchar_t (&str1)[lengthInCharacters] );

//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	template <size_t lengthInCharacters>
	ETPureFunctionHint bool	StringsEqual( const wchar_t (&str0)[lengthInCharacters], const wchar_t (&str1)[lengthInCharacters] );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	template <size_t lengthInCharacters>
	ETPureFunctionHint bool	StringsEqual( const char (&str0)[lengthInCharacters], const char (&str1)[lengthInCharacters] );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	StringsEqual( const wchar_t* str0, const wchar_t* str1 );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	StringsEqual( const char* str0, const char* str1 );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	StringsEqual( const wchar_t* str0, const wchar_t* str1, size_t lengthInCharacters );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	StringsEqual( const char* str0, const char* str1, size_t lengthInCharacters );

	ETPureFunctionHint bool	StringsEqualCaseInsensitive( const wchar_t* str0, const wchar_t* str1, size_t lengthInCharacters );
	ETPureFunctionHint bool	StringsEqualCaseInsensitive( const char* str0, const char* str1, size_t lengthInCharacters );
	ETPureFunctionHint bool	StringsEqualCaseInsensitive( const wchar_t* str0, const wchar_t* str1 );
	ETPureFunctionHint bool	StringsEqualCaseInsensitive( const char* str0, const char* str1 );

	template <size_t lengthInCharacters>
	ETPureFunctionHint bool	StringsEqualCaseInsensitive( const wchar_t (&str0)[lengthInCharacters], const wchar_t (&str1)[lengthInCharacters] );
	template <size_t lengthInCharacters>
	ETPureFunctionHint bool	StringsEqualCaseInsensitive( const char (&str0)[lengthInCharacters], const char (&str1)[lengthInCharacters] );

// ---------------------------------------------------

	ETPureFunctionHint size_t	HashMemory( const void* memory, size_t sizeInBytes, size_t seed = 0u );

// ---------------------------------------------------

//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	template <size_t lengthInCharacters>
	ETPureFunctionHint size_t	StringLength( const wchar_t (&str)[lengthInCharacters] );
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	template <size_t lengthInCharacters>
	ETPureFunctionHint size_t	StringLength( const char (&str)[lengthInCharacters] );
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	ETPureFunctionHint size_t	StringLength( const wchar_t* string );
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	ETPureFunctionHint size_t	StringLength( const char* string );

// ---------------------------------------------------

//!	Copies the C string specified by src into the array pointed to by destinationString, including the terminator.
	template <size_t lengthInCharacters>
	ETPureFunctionHint auto		CopyString( wchar_t (&destinationString)[lengthInCharacters], const wchar_t* sourceString ) -> decltype(destinationString);
//!	Copies the C string specified by src into the array pointed to by destinationString, including the terminator.
	template <size_t lengthInCharacters>
	ETPureFunctionHint auto		CopyString( char (&destinationString)[lengthInCharacters], const char* sourceString ) -> decltype(destinationString);
//!	Copies the C string specified by src into the array pointed to by destinationString, including the terminator.
	ETPureFunctionHint wchar_t*	CopyString( wchar_t* destinationString, const wchar_t* src, size_t maxLengthInCharacters );
//!	Copies the C string specified by src into the array pointed to by dst, including the terminator.
	ETPureFunctionHint char*	CopyString( char* dst, const char* src, size_t maxLengthInCharacters );

// ---------------------------------------------------

//! Appends the C string specified by sourceString after the C string denoted by destinationString.
	template <size_t lengthInCharacters>
	ETPureFunctionHint auto		AppendString( wchar_t (&destinationString)[lengthInCharacters], const wchar_t* sourceString ) -> decltype(destinationString);
//!	Appends the C string specified by sourceString after the C string denoted by destinationString.
	template <size_t lengthInCharacters>
	ETPureFunctionHint auto		AppendString( char (&destinationString)[lengthInCharacters], const char* sourceString ) -> decltype(destinationString);
//!	Appends the C string specified by sourceString after the C string denoted by destinationString.
	ETPureFunctionHint wchar_t*	AppendString( wchar_t* destinationString, const wchar_t* sourceString, size_t maxLengthInCharacters );
//!	Appends the C string specified by sourceString after the C string denoted by destinationString.
	ETPureFunctionHint char*	AppendString( char* destinationString, const char* sourceString, size_t maxLengthInCharacters );

// ---------------------------------------------------

//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const char*		FindFirstInstance( const char* string, const char character );
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint char*			FindFirstInstance( char* string, const char character );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint const char*		FindFirstInstance( const char* string, const char* substring );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint char*			FindFirstInstance( char* string, const char* substring );
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindFirstInstance( const wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindFirstInstance( wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindFirstInstance( const wchar_t* string, const wchar_t* substring );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindFirstInstance( wchar_t* string, const wchar_t* substring );

//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const char*		FindLastInstance( const char* string, const char character );
//	Locates/returns a pointer to the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint char*			FindLastInstance( char* string, const char character );
	//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
	//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint const char*		FindLastInstance( const char* string, const char* substring );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint char*			FindLastInstance( char* string, const char* substring );
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindLastInstance( const wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindLastInstance( wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindLastInstance( const wchar_t* string, const wchar_t* substring );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindLastInstance( wchar_t* string, const wchar_t* substring );

//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint const wchar_t*	FindTerminator( const wchar_t* string );
//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint wchar_t*			FindTerminator( wchar_t* string );
//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint const char*		FindTerminator( const char* string );
//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint char*			FindTerminator( char* string );

	ETPureFunctionHint const wchar_t*	TrimLeadingWhitespace( const wchar_t* string, const wchar_t* const stringEnd );
	ETPureFunctionHint const char*		TrimLeadingWhitespace( const char* string, const char* const stringEnd );
	
	ETPureFunctionHint const wchar_t*	TrimTrailingWhitespace( const wchar_t* string, const wchar_t* const stringEnd );
	ETPureFunctionHint const char*		TrimTrailingWhitespace( const char* string, const char* const stringEnd );
	
	ETPureFunctionHint const wchar_t*	EmptyForNull( const wchar_t* string );
	ETPureFunctionHint const char*		EmptyForNull( const char* string );

// ---------------------------------------------------

/*!	Attempts to locate a pointer to the first 'different' element in array0 as compared to array1.
 *	If the arrays are identical, a null pointer is returned instead. */
	template <typename T>
	ETPureFunctionHint const T*	FindArrayDifference( const T* ETRestrictPtrHint array0, const T* ETRestrictPtrHint array1, size_t sizeInElements );
/*!	Attempts to locate a pointer to the first 'different' element in array0 as compared to array1.
 *	If the arrays are identical, a null pointer is returned instead. */
	template <typename T>
	ETPureFunctionHint T*		FindArrayDifference( T* ETRestrictPtrHint array0, T* ETRestrictPtrHint array1, size_t sizeInElements );
/*!	Attempts to locate a pointer to the first 'different' element in array0 as compared to array1.
 *	If the arrays are identical, a null pointer is returned instead. */
	template <typename T, size_t sizeInElements>
	ETPureFunctionHint const T*	FindArrayDifference( const T (&array0)[sizeInElements], const T (&array1)[sizeInElements] );

}	// namespace Eldritch2

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
//------------------------------------------------------------------//