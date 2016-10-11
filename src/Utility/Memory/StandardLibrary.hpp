/*==================================================================*\
  StandardLibrary.hpp
  ------------------------------------------------------------------
  Purpose:
  Utility functions that serve as replacements for standard C bulk
  memory operations and may be faster (but are guaranteed to be
  equivalent to the compiler's at worst) than their ANSI equivalent.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
#include <Utility/MPL/Platform.hpp>
//------------------------------------------------------------------//
#include <EABase/eastdarg.h>
//------------------------------------------------------------------//

// Thanks for polluting the global namespace with your macros, Microsoft!
#if defined( CopyMemory )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_CopyMemory 1
#		pragma push_macro( "CopyMemory" )
#		undef CopyMemory
#	else
#		error CopyMemory macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif
#if defined( MoveMemory )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_MoveMemory 1
#		pragma push_macro( "MoveMemory" )
#		undef MoveMemory
#	else
#		error MoveMemory macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif
#if defined( SetMemory )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_SetMemory 1
#		pragma push_macro( "SetMemory" )
#		undef SetMemory
#	else
#		error SetMemory macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif
#if defined( ZeroMemory )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_ZeroMemory 1
#		pragma push_macro( "ZeroMemory" )
#		undef ZeroMemory
#	else
#		error ZeroMemory macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif
#if defined( CompareString )
#	if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#		define POP_CompareString 1
#		pragma push_macro( "CompareString" )
#		undef CompareString
#	else
#		error CompareString macro creates symbol/name resolution errors, replace me with cleanup code!
#	endif
#endif

namespace Eldritch2 {

//! Copies a span of bytes.
/*!	@param[in] destinationBuffer Pointer to start copying memory to.
	@param[in] sourceBuffer Pointer to start copying memory from.
	@param[in] lengthInBytes Number of bytes to copy from _sourceBuffer_ to _destinationBuffer_.
	@pre The regions defined by [_destinationBuffer_, _destinationBuffer_ + _lengthInBytes_), [_sourceBuffer_, _sourceBuffer_ + _lengthInBytes_) should not overlap; if this is a possibility use MoveMemory() instead.
	@see CopyMemoryNonTemporal()
	@see MoveMemory() */
	ETPureFunctionHint void*	CopyMemory( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, const size_t lengthInBytes );

//! Copies a span of bytes.
/*! @param[in] destinationBuffer Pointer to start copying memory to.
	@param[in] sourceBuffer Pointer to start copying memory from.
	@param[in] lengthInBytes Number of bytes to copy from _sourceBuffer_ to _destinationBuffer_.
	@remark Uses non-temporal (unlikely to be accessed again soon) cache hinting semantics.
	@pre The regions defined by [_destinationBuffer_, _destinationBuffer_ + _lengthInBytes_), [_sourceBuffer_, _sourceBuffer_ + _lengthInBytes_) should not overlap; if this is a possibility use MoveMemoryNonTemporal() instead.
	@see CopyMemory()
	@see MoveMemoryNonTemporal() */
	ETPureFunctionHint void*	CopyMemoryNonTemporal( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, const size_t lengthInBytes );

//!	Utility wrapper around @ref CopyMemory() that handles casting and automatically adjusts for element size.
	template <typename T>
	ETPureFunctionHint T*	CopyArray( T* ETRestrictPtrHint destinationArray, const T* ETRestrictPtrHint sourceArray, const size_t arraySizeInElements );
//!	Utility wrapper around @ref CopyMemory() that handles casting and automatically adjusts for element size.
	template <typename T, size_t arraySizeInElements>
	ETPureFunctionHint auto	CopyArray( T(&destinationArray)[arraySizeInElements], const T(&sourceArray)[arraySizeInElements] ) -> decltype(destinationArray);

//!	Utility wrapper around @ref CopyMemoryNonTemporal that handles casting and automatically adjusts for element size.
	template <typename T>
	ETPureFunctionHint T*	CopyArrayNonTemporal( T* ETRestrictPtrHint destinationArray, const T* ETRestrictPtrHint sourceArray, const size_t arraySizeInElements );
//!	Utility wrapper around @ref CopyMemoryNonTemporal that handles casting and automatically adjusts for element size.
	template <typename T, size_t arraySizeInElements>
	ETPureFunctionHint auto	CopyArrayNonTemporal( T(&destinationArray)[arraySizeInElements], const T(&sourceArray)[arraySizeInElements] ) -> decltype(destinationArray);

// ---------------------------------------------------

//!	Copies bytes from the address specified by source to the location specified by destination.
/*!	Unlike @ref CopyMemory(), the source and destination buffers may overlap. */
	ETPureFunctionHint void*	MoveMemory( void* destinationBuffer, const void* sourceBuffer, const size_t lengthInBytes );

//!	Copies bytes from the address specified by source to the location specified by destination with non-temporal (unlikely to be accessed again soon) semantics.
/*!	As with @ref MoveMemory() and unlike @ref CopyMemory(), the source and destination arrays may overlap. */
	ETPureFunctionHint void*	MoveMemoryNonTemporal( void* destinationBuffer, const void* sourceBuffer, const size_t lengthInBytes );

// ---------------------------------------------------

//!	Sets the value of length bytes in the array specified by destination to the bit pattern denoted in bitPattern.
	ETPureFunctionHint void*	SetMemory( void* destinationBuffer, const int bitPattern, const size_t lengthInBytes );

//!	Zeroes out the entirety of numElements instances of type T.
	template <typename T>
	ETPureFunctionHint T*	ZeroMemory( T* destinationArray, size_t arraySizeInElements );

//!	Zeroes out the entirety of numElements instances of type T.
	template <typename T, size_t arraySizeInElements>
	ETPureFunctionHint auto	ZeroMemory( T (&destinationArray)[arraySizeInElements] ) -> decltype(destinationArray);

// ---------------------------------------------------

//	Performs a raw, bitwise comparison of the first span bytes pointed to by buffer0 against the first span bytes pointed to
//	by buffer1. If all bytes are equal, the result is 0. Otherwise, the result is greater than zero if the first byte that does
//	not match in buffer0 is greater than the first byte that does not match in buffer1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareMemory( const void* buffer0, const void* buffer1, const size_t spanInBytes );

//	Performs a raw, bitwise comparison of the first span bytes pointed to by buffer0 against the first span bytes pointed to
//	by buffer1. If all bytes are equal, the result is 0. Otherwise, the result is greater than zero if the first byte that does
//	not match in buffer0 is greater than the first byte that does not match in buffer1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareMemoryCaseInsensitive( const void* buffer0, const void* buffer1, const size_t spanInBytes );

//!	Utility wrapper around @ref CompareMemory() that handles casting and automatically adjusts for element size.
	template <typename T>
	ETPureFunctionHint int	CompareArray( const T* array0, const T* array1, const size_t arraySizeInElements );
//!	Utility wrapper around @ref CompareMemory() that handles casting and automatically adjusts for element size.
	template <typename T, size_t arraySizeInElements>
	ETPureFunctionHint int	CompareArray( const T(&array0)[arraySizeInElements], const T(&array1)[arraySizeInElements] );

//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareString( const char* string0, const char* string1 );
//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareString( const wchar_t* string0, const wchar_t* string1 );
//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint int	CompareString( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] );
//	Performs a raw, bitwise comparison of the individual characters in string0 against the individual characters in string1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in string0 is greater than the first character that does not match in string1 and vice versa in the less-than case.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint int	CompareString( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] );

//	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareStringCaseInsensitive( const char* str0, const char* str1 );
//	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareStringCaseInsensitive( const wchar_t* str0, const wchar_t* str1 );
//	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareStringCaseInsensitive( const char* str0, const char* str1, const size_t stringLengthInCharacters );
//	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case.
	ETPureFunctionHint int	CompareStringCaseInsensitive( const wchar_t* str0, const wchar_t* str1, const size_t stringLengthInCharacters );
//	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint int	CompareStringCaseInsensitive( const char (&str0)[stringSizeInCharacters], const char (&str1)[stringSizeInCharacters] );
//	Performs a case-insensitive comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is 0. Otherwise, the result is greater than zero if the first character that does
//	not match in str0 is greater than the first character that does not match in str1 and vice versa in the less-than case.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint int	CompareStringCaseInsensitive( const wchar_t (&str0)[stringSizeInCharacters], const wchar_t (&str1)[stringSizeInCharacters] );

//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	EqualityCompareString( const char* str0, const char* str1 );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	EqualityCompareString( const wchar_t* str0, const wchar_t* str1 );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	EqualityCompareString( const char* str0, const char* str1, const size_t lengthInCharacters );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	ETPureFunctionHint bool	EqualityCompareString( const wchar_t* str0, const wchar_t* str1, const size_t lengthInCharacters );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint bool	EqualityCompareString( const char (&str0)[stringSizeInCharacters], const char (&str1)[stringSizeInCharacters] );
//	Performs a raw, bitwise comparison of the individual characters in str0 against the individual characters in str1.
//	If all characters are equal, the result is true, otherwise the function returns false.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint bool	EqualityCompareString( const wchar_t (&str0)[stringSizeInCharacters], const wchar_t (&str1)[stringSizeInCharacters] );

	ETPureFunctionHint bool	EqualityCompareStringCaseInsensitive( const char* str0, const char* str1 );
	ETPureFunctionHint bool	EqualityCompareStringCaseInsensitive( const wchar_t* str0, const wchar_t* str1 );
	ETPureFunctionHint bool	EqualityCompareStringCaseInsensitive( const char* str0, const char* str1, const size_t lengthInCharacters );
	ETPureFunctionHint bool	EqualityCompareStringCaseInsensitive( const wchar_t* str0, const wchar_t* str1, const size_t lengthInCharacters );

	template <size_t stringSizeInCharacters>
	ETPureFunctionHint bool	EqualityCompareStringCaseInsensitive( const char (&str0)[stringSizeInCharacters], const char (&str1)[stringSizeInCharacters] );
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint bool	EqualityCompareStringCaseInsensitive( const wchar_t (&str0)[stringSizeInCharacters], const wchar_t (&str1)[stringSizeInCharacters] );

// ---------------------------------------------------

	ETPureFunctionHint size_t	HashMemory( const void* memory, size_t sizeInBytes, size_t seed = 0u );

// ---------------------------------------------------

//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	ETPureFunctionHint size_t	StringLength( const char* string );
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	ETPureFunctionHint size_t	StringLength( const wchar_t* string );
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint size_t	StringLength( const char (&str)[stringSizeInCharacters] );
//!	Given a character array, determine the number of characters in the array before the first instance of a null terminator.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint size_t	StringLength( const wchar_t (&str)[stringSizeInCharacters] );

// ---------------------------------------------------

//!	Copies the C string specified by src into the array pointed to by dst, including the terminator.
	ETPureFunctionHint char*		CopyString( char* dst, const char* src, const size_t maxLengthInCharacters );
//!	Copies the C string specified by src into the array pointed to by destinationString, including the terminator.
	ETPureFunctionHint wchar_t*	CopyString( wchar_t* destinationString, const wchar_t* src, const size_t maxLengthInCharacters );
//!	Copies the C string specified by src into the array pointed to by destinationString, including the terminator.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint auto		CopyString( char (&destinationString)[stringSizeInCharacters], const char* sourceString ) -> decltype(destinationString);
//!	Copies the C string specified by src into the array pointed to by destinationString, including the terminator.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint auto		CopyString( wchar_t (&destinationString)[stringSizeInCharacters], const wchar_t* sourceString ) -> decltype(destinationString);

// ---------------------------------------------------

//!	Appends the C string specified by sourceString after the C string denoted by destinationString.
	ETPureFunctionHint char*		AppendString( char* destinationString, const char* sourceString, const size_t maxLengthInCharacters );
//!	Appends the C string specified by sourceString after the C string denoted by destinationString.
	ETPureFunctionHint wchar_t*	AppendString( wchar_t* destinationString, const wchar_t* sourceString, const size_t maxLengthInCharacters );

//!	Appends the C string specified by sourceString after the C string denoted by destinationString.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint auto		AppendString( char (&destinationString)[stringSizeInCharacters], const char* sourceString ) -> decltype(destinationString);
//! Appends the C string specified by sourceString after the C string denoted by destinationString.
	template <size_t stringSizeInCharacters>
	ETPureFunctionHint auto		AppendString( wchar_t (&destinationString)[stringSizeInCharacters], const wchar_t* sourceString ) -> decltype(destinationString);

// ---------------------------------------------------

//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint char*				FindFirstInstance( char* string, const char character );
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const char*		FindFirstInstance( const char* string, const char character );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint char*				FindFirstInstance( char* string, const char* substring );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint const char*		FindFirstInstance( const char* string, const char* substring );
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindFirstInstance( wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to the first instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindFirstInstance( const wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindFirstInstance( wchar_t* string, const wchar_t* substring );
//	Locates/returns a pointer to the first instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindFirstInstance( const wchar_t* string, const wchar_t* substring );

//	Locates/returns a pointer to the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint char*				FindLastInstance( char* string, const char character );
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const char*		FindLastInstance( const char* string, const char character );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint char*				FindLastInstance( char* string, const char* substring );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the sequence is not present in the string.
	ETPureFunctionHint const char*		FindLastInstance( const char* string, const char* substring );
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindLastInstance( wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to  the last instance of the character passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindLastInstance( const wchar_t* string, const wchar_t character );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint wchar_t*			FindLastInstance( wchar_t* string, const wchar_t* substring );
//	Locates/returns a pointer to  the last instance of the string passed via the second parameter in the string denoted by the first parameter,
//	returning a null pointer if the character is not present in the string.
	ETPureFunctionHint const wchar_t*	FindLastInstance( const wchar_t* string, const wchar_t* substring );

//	 Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	 an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint char*				FindEndOfString( char* string );
//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint const char*		FindEndOfString( const char* string );
//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint wchar_t*			FindEndOfString( wchar_t* string );
//	Locates/returns a pointer to the end of the passed-in C string. This function assumes that the string is, in fact, null-terminated; passing
//	an unterminated string may result in a buffer read overrun.
	ETPureFunctionHint const wchar_t*	FindEndOfString( const wchar_t* string );

	ETPureFunctionHint const char*		StripLeadingWhitespace( const char* string, const char* const stringEnd );
	ETPureFunctionHint const wchar_t*	StripLeadingWhitespace( const wchar_t* string, const wchar_t* const stringEnd );

	ETPureFunctionHint const char*		StripTrailingWhitespace( const char* string, const char* const stringEnd );
	ETPureFunctionHint const wchar_t*	StripTrailingWhitespace( const wchar_t* string, const wchar_t* const stringEnd );

// ---------------------------------------------------

//	Given a destination buffer, a format string and a number of additional arguments, attempt to fill the destination buffer
//	according to the format rules laid out by the C standard library function printf/sprintf.
	ETPureFunctionHint char*		PrintFormatted( char* destinationString, const size_t maxCharacters, const char* const formatString, ... );
//	Given a destination buffer, a format string and a number of additional arguments, attempt to fill the destination buffer
//	according to the format rules laid out by the C standard library function printf/sprintf.
	ETPureFunctionHint wchar_t*	PrintFormatted( wchar_t* destinationString, const size_t maxCharacters, const wchar_t* const formatString, ... );
//	Given a destination buffer, a format string and a number of additional arguments, attempt to fill the destination buffer
//	according to the format rules laid out by the C standard library function printf/sprintf.
	template <typename Character, size_t stringSizeInCharacters>
	ETPureFunctionHint auto		PrintFormatted( Character (&destinationString)[stringSizeInCharacters], const Character* const formatString, ... ) -> decltype(destinationString);
//	Given a destination buffer, a format string and a number of additional arguments, attempt to fill the destination buffer
//	according to the format rules laid out by the C standard library function printf/sprintf.
	ETPureFunctionHint char*		PrintFormatted( char* destinationString, const size_t maxCharacters, const char* const formatString, va_list args );
//	Given a destination buffer, a format string and a number of additional arguments, attempt to fill the destination buffer
//	according to the format rules laid out by the C standard library function printf/sprintf.
	ETPureFunctionHint wchar_t*	PrintFormatted( wchar_t* destinationString, const size_t maxCharacters, const wchar_t* const formatString, va_list args );
//	Given a destination buffer, a format string and a number of additional arguments, attempt to fill the destination buffer
//	according to the format rules laid out by the C standard library function printf/sprintf.
	template <typename Character, size_t stringSizeInCharacters>
	ETPureFunctionHint auto		PrintFormatted( Character (&destinationString)[stringSizeInCharacters], const Character* const formatString, va_list args ) -> decltype(destinationString);

// ---------------------------------------------------

//!	Attempts to locate a pointer to the first 'different' element in array0 as compared to array1.
//	If the arrays are identical, a null pointer is returned instead.
	template <typename T>
	ETPureFunctionHint T*		SeekArrayDifference( T* ETRestrictPtrHint array0, T* ETRestrictPtrHint array1, const size_t arraySizeInElements );
//!	Attempts to locate a pointer to the first 'different' element in array0 as compared to array1.
//	If the arrays are identical, a null pointer is returned instead.
	template <typename T>
	ETPureFunctionHint const T*	SeekArrayDifference( const T* ETRestrictPtrHint array0, const T* ETRestrictPtrHint array1, const size_t arraySizeInElements );
//!	 Attempts to locate a pointer to the first 'different' element in array0 as compared to array1.
//	 If the arrays are identical, a null pointer is returned instead.
	template <typename T, size_t arraySizeInElements>
	ETPureFunctionHint const T*	SeekArrayDifference( const T (&array0)[arraySizeInElements], const T (&array1)[arraySizeInElements] );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#if( ET_PLATFORM_WINDOWS )
#	include <Utility/Memory/StandardLibrary.Win.inl>
#elif( ET_PLATFORM_MAC )
#	include <Utility/Memory/StandardLibrary.Mac.inl>
#elif( ET_PLATFORM_LINUX )
#	include <Utility/Memory/StandardLibrary.Linux.inl>
#endif
//------------------------------------------------------------------//

#if( ET_COMPILER_IS_MSVC || ET_COMPILER_IS_GCC )
#	ifdef POP_CopyMemory
#		pragma pop_macro( "CopyMemory" )
#		undef POP_CopyMemory
#	endif
#	ifdef POP_MoveMemory
#		pragma pop_macro( "MoveMemory" )
#		undef POP_MoveMemory
#	endif
#	ifdef POP_SetMemory
#		pragma pop_macro( "SetMemory" )
#		undef POP_SetMemory
#	endif
#	ifdef POP_ZeroMemory
#		pragma pop_macro( "ZeroMemory" )
#		undef POP_ZeroMemory
#	endif
#endif