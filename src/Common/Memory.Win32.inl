/*==================================================================*\
  Memory.Win32.inl
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

	ETForceInlineHint ETPureFunctionHint void* CopyMemory( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, size_t lengthInBytes ) {
		return std::memcpy( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* CopyMemoryNonTemporal( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, size_t lengthInBytes ) {
		return std::memcpy( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T* CopyArray( T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, size_t sizeInElements ) {
		return static_cast<T*>(Eldritch2::CopyMemory( static_cast<void*>(destination), static_cast<const void*>(source), sizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	template <typename T, size_t sizeInElements>
	ETForceInlineHint ETPureFunctionHint auto CopyArray( T (&destinationArray)[sizeInElements], const T (&sourceArray)[sizeInElements] ) -> decltype(destinationArray) {
		return (Eldritch2::CopyArray( destinationArray, sourceArray, sizeInElements ), destinationArray);
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T* CopyArrayNonTemporal( T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, size_t sizeInElements ) {
		return static_cast<T*>(Eldritch2::CopyMemoryNonTemporal( static_cast<void*>(destination), static_cast<const void*>(source), sizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	template <typename T, size_t sizeInElements>
	ETForceInlineHint ETPureFunctionHint auto CopyArrayNonTemporal( T (&destination)[sizeInElements], const T (&source)[sizeInElements] ) -> decltype(destination) {
		return (Eldritch2::CopyArrayNonTemporal( destination, source, sizeInElements ), destination);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* MoveMemory( void* destinationBuffer, const void* sourceBuffer, size_t lengthInBytes ) {
		return std::memmove( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* MoveMemoryNonTemporal( void* destinationBuffer, const void* sourceBuffer, size_t lengthInBytes ) {
		return Eldritch2::MoveMemory( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* SetMemory( void* destination, int bitPattern, size_t lengthInBytes ) {
		return std::memset( destination, bitPattern, lengthInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T* ZeroMemory( T* destination, size_t sizeInElements ) {
		static_assert( eastl::is_trivially_copyable<T>::value, "Zeroing polymorphic types destroys virtual function tables and is disabled." );

	// ---

		return static_cast<T*>(Eldritch2::SetMemory( destination, 0, sizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

//	Semi-hack: Define a specialization for the void type for compatibility with the more conventional usage
	template <>
	ETForceInlineHint ETPureFunctionHint void* ZeroMemory<void>( void* destination, size_t bufferSizeInBytes ) {
		return Eldritch2::SetMemory( destination, 0, bufferSizeInBytes );
	}

// ---------------------------------------------------

	template <typename T, size_t sizeInElements>
	ETForceInlineHint ETPureFunctionHint auto ZeroMemory( T (&destination)[sizeInElements] ) -> decltype(destination) {
		return (Eldritch2::ZeroMemory( static_cast<T*>(destination), sizeInElements ), destination);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderBuffersCaseInsensitive( const void* buffer0, const void* buffer1, size_t spanInBytes ) {
		return _memicmp( buffer0, buffer1, spanInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderBuffers( const void* buffer0, const void* buffer1, size_t spanInBytes ) {
		return std::memcmp( buffer0, buffer1, spanInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint int OrderArrays( const T* array0, const T* array1, size_t sizeInElements ) {
		return Eldritch2::CompareMemory( static_cast<const void*>(array0), static_cast<const void*>(array1), sizeInElements * sizeof(T) );
	}

// ---------------------------------------------------

	template <typename T, size_t sizeInElements>
	ETForceInlineHint ETPureFunctionHint int OrderArrays( const T (&array0)[sizeInElements], const T (&array1)[sizeInElements] ) {
		return Eldritch2::CompareArray( static_cast<const T*>(array0), static_cast<const T*>(array1), sizeInElements );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderStrings( const char* string0, const char* string1 ) {
		return std::strcmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderStrings( const wchar_t* string0, const wchar_t* string1 ) {
		return std::wcscmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderStrings( const char* string0, const char* string1, size_t lengthInCharacters ) {
		return std::strncmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderStrings( const wchar_t* string0, const wchar_t* string1, size_t lengthInCharacters ) {
		return std::wcsncmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint int OrderStrings( const char (&string0)[lengthInCharacters], const char (&string1)[lengthInCharacters] ) {
		return Eldritch2::OrderStrings( static_cast<const char*>(string0), static_cast<const char*>(string1), lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint int OrderStrings( const wchar_t (&string0)[lengthInCharacters], const wchar_t (&string1)[lengthInCharacters] ) {
		return Eldritch2::OrderStrings( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive( const char* string0, const char* string1 ) {
		return _stricmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive( const wchar_t* string0, const wchar_t* string1 ) {
		return _wcsicmp( string0, string1 );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive( const char( &string0 )[lengthInCharacters], const char( &string1 )[lengthInCharacters] ) {
		return std::_memicmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint int OrderStringsCaseInsensitive( const wchar_t( &string0 )[lengthInCharacters], const wchar_t( &string1 )[lengthInCharacters] ) {
		return Eldritch2::OrderStringsCaseInsensitive( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1) );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqual( const char* string0, const char* string1 ) {
		return 0 == Eldritch2::OrderStrings( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqual( const wchar_t* string0, const wchar_t* string1 ) {
		return 0 == Eldritch2::OrderStrings( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqual( const char* string0, const char* string1, size_t lengthInCharacters ) {
		return 0 == Eldritch2::OrderStrings( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqual( const wchar_t* string0, const wchar_t* string1, size_t lengthInCharacters ) {
		return 0 == Eldritch2::OrderStrings( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint bool StringsEqual( const char (&string0)[lengthInCharacters], const char (&string1)[lengthInCharacters] ) {
		return Eldritch2::StringsEqual( static_cast<const char*>(string0), static_cast<const char*>(string1), lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint bool StringsEqual( const wchar_t (&string0)[lengthInCharacters], const wchar_t (&string1)[lengthInCharacters] ) {
		return Eldritch2::StringsEqual( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive( const char* string0, const char* string1 ) {
		return 0 == Eldritch2::OrderStringsCaseInsensitive( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive( const wchar_t* string0, const wchar_t* string1 ) {
		return 0 == Eldritch2::OrderStringsCaseInsensitive( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive( const char* string0, const char* string1, size_t lengthInCharacters ) {
		return 0 == Eldritch2::OrderStringsCaseInsensitive( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive( const wchar_t* string0, const wchar_t* string1, size_t lengthInCharacters ) {
		return 0 == Eldritch2::OrderStringsCaseInsensitive( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive( const char (&string0)[lengthInCharacters], const char (&string1)[lengthInCharacters] ) {
		return Eldritch2::StringsEqualCaseInsensitive( static_cast<const char*>(string0), static_cast<const char*>(string1), lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint bool StringsEqualCaseInsensitive( const wchar_t (&string0)[lengthInCharacters], const wchar_t (&string1)[lengthInCharacters] ) {
		return Eldritch2::StringsEqualCaseInsensitive( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint size_t StringLength( const char* string ) {
		return std::strlen( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint size_t StringLength( const wchar_t* string ) {
		return std::wcslen( string );
	}

// ---------------------------------------------------

	template<size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint size_t StringLength( const char (&string)[lengthInCharacters] ) {
		return Eldritch2::StringLength( static_cast<const char*>(string) );
	}

// ---------------------------------------------------

	template<size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint size_t StringLength( const wchar_t (&string)[lengthInCharacters] ) {
		return Eldritch2::StringLength( static_cast<const wchar_t*>(string) );
	}

// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
//	(4996) MSVC yells at us for using the insecure string functions here.
#	pragma warning( push )
#	pragma warning( disable : 4996 )
#endif

	ETForceInlineHint ETPureFunctionHint char* CopyString( char* destinationString, const char* sourceString, size_t maxLengthInCharacters ) {
		return std::strncpy( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* CopyString( wchar_t* destinationString, const wchar_t* sourceString, size_t maxLengthInCharacters ) {
		return std::wcsncpy( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint auto CopyString( char (&destinationString)[lengthInCharacters], const char* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::CopyString( static_cast<char*>(destinationString), sourceString, lengthInCharacters ), destinationString);
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint auto CopyString( wchar_t (&destinationString)[lengthInCharacters], const wchar_t* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::CopyString( static_cast<wchar_t*>(destinationString), sourceString, lengthInCharacters ), destinationString);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* AppendString( char* destinationString, const char* sourceString, size_t maxLengthInCharacters ) {
		return std::strncat( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* AppendString( wchar_t* destinationString, const wchar_t* sourceString, size_t maxLengthInCharacters ) {
		return std::wcsncat( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint auto AppendString( char (&destinationString)[lengthInCharacters], const char* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::AppendString( static_cast<char*>(destinationString), sourceString, lengthInCharacters ), destinationString);
	}

// ---------------------------------------------------

	template <size_t lengthInCharacters>
	ETForceInlineHint ETPureFunctionHint auto AppendString( wchar_t( &destinationString )[lengthInCharacters], const wchar_t* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::AppendString( static_cast<wchar_t*>(destinationString), sourceString, lengthInCharacters ), destinationString);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* FindFirstInstance( char* string, const char character ) {
		return std::strchr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint const char* FindFirstInstance( const char* string, const char character ) {
		return std::strchr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* FindFirstInstance( char* string, const char* substring ) {
		return std::strstr( string, substring );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* FindFirstInstance( const char* string, const char* substring ) {
		return std::strstr( string, substring );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint wchar_t* FindFirstInstance( wchar_t* string, const wchar_t character ) {
		return std::wcschr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint const wchar_t* FindFirstInstance( const wchar_t* string, const wchar_t character ) {
		return std::wcschr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* FindFirstInstance( wchar_t* string, const wchar_t* substring ) {
		return std::wcsstr( string, substring );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* FindFirstInstance( const wchar_t* string, const wchar_t* substring ) {
		return std::wcsstr( string, substring );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint char* FindLastInstance( char* string, const char character ) {
		return std::strrchr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint const char* FindLastInstance( const char* string, const char character ) {
		return std::strrchr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* FindLastInstance( char* string, const char* substring ) {
		return const_cast<char*>(Eldritch2::FindLastInstance( const_cast<const char*>(string), substring ));
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* FindLastInstance( const char* string, const char* substring ) {
		if (!substring || ('\0' == substring[0])) {
			return nullptr;
		}

		const auto	substringLength( Eldritch2::StringLength( substring ) );
		const auto	stringLength( Eldritch2::StringLength( string ) );
		
		if (stringLength < substringLength) {
			return nullptr;
		}	

		for (auto cursor( string + stringLength - substringLength ); cursor >= string; --cursor) {
			if (StringsEqual( cursor, substring, substringLength )) {
				return cursor;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint wchar_t* FindLastInstance( wchar_t* string, const wchar_t character ) {
		return std::wcsrchr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETPureFunctionHint const wchar_t* FindLastInstance( const wchar_t* string, const wchar_t character ) {
		return std::wcsrchr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* FindLastInstance( wchar_t* string, const wchar_t* substring ) {
		return const_cast<wchar_t*>(Eldritch2::FindLastInstance( const_cast<const wchar_t*>(string), substring ));
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* FindLastInstance( const wchar_t* string, const wchar_t* substring ) {
		if (!substring || (L'\0' == substring[0])) {
			return nullptr;
		}

		const auto	substringLength( StringLength( substring ) );
		const auto	stringLength( StringLength( string ) );

		if (stringLength < substringLength) {
			return nullptr;
		}

		for (auto cursor( string + stringLength - substringLength ); cursor >= string; --cursor) {
			if (StringsEqual( cursor, substring, substringLength )) {
				return cursor;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* FindTerminator( char* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* FindTerminator( const char* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* FindTerminator( wchar_t* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* FindTerminator( const wchar_t* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* TrimLeadingWhitespace( const char* string, const char* const stringEnd ) {
		for( ; string != stringEnd && (0 != std::isspace( static_cast<int>(*string) )); ++string ) {}

		return string;
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* TrimLeadingWhitespace( const wchar_t* string, const wchar_t* const stringEnd ) {
		for( ; string != stringEnd && (0 != std::iswspace( static_cast<::wint_t>(*string) )); ++string ) {}

		return string;
	}

// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
//	MSVC complains needlessly about a harmless type promotion for std::isspace().
#	pragma warning( push )
#	pragma warning( disable : 6330 )
#endif
	ETForceInlineHint ETPureFunctionHint const char* TrimTrailingWhitespace( const char* const string, const char* stringEnd ) {
		for( ; string != stringEnd && std::isspace( stringEnd[-1] ); --stringEnd ) {}

		return stringEnd;
	}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* TrimTrailingWhitespace( const wchar_t* const string, const wchar_t* stringEnd ) {
		for( ; string != stringEnd && std::iswspace( stringEnd[-1] ); --stringEnd ) {}

		return stringEnd;
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* EmptyForNull( const wchar_t* string ) {
		return string ? string : L"";
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* EmptyForNull( const char* string ) {
		return string ? string : "";
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint const T* FindArrayDifference( T* ETRestrictPtrHint array0, T* ETRestrictPtrHint array1, size_t sizeInElements ) {
		for( T* ETRestrictPtrHint const end( array0 + sizeInElements ); array0 != end; ++array0, ++array1 ) {
			if( *array0 != *array1 ) {
				return array0;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint const T* FindArrayDifference( const T* ETRestrictPtrHint array0, const T* ETRestrictPtrHint array1, size_t sizeInElements ) {
		for( const T* ETRestrictPtrHint const end( array0 + sizeInElements ); array0 != end; ++array0, ++array1 ) {
			if( *array0 != *array1 ) {
				return array0;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	template <typename T, size_t sizeInElements>
	ETForceInlineHint ETPureFunctionHint T* FindArrayDifference( T (&array0)[sizeInElements], T (&array1)[sizeInElements] ) {
		return Eldritch2::FindArrayDifference( static_cast<T*>(array0), static_cast<T*>(array1), sizeInElements );
	}

// ---------------------------------------------------

	template <typename T, size_t sizeInElements>
	ETForceInlineHint ETPureFunctionHint const T* FindArrayDifference( const T(&array0)[sizeInElements], const T(&array1)[sizeInElements] ) {
		return Eldritch2::FindArrayDifference( static_cast<const T*>(array0), static_cast<const T*>(array1), sizeInElements );
	}

}	// namespace Eldritch2