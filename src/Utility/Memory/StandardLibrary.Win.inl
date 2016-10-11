/*==================================================================*\
  StandardLibrary.Win.inl
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
#include <cstdarg>
#include <cstring>
#include <cwctype>
#include <cctype>
#include <cstdio>
#include <cwchar>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETForceInlineHint ETPureFunctionHint void* CopyMemory( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, const size_t lengthInBytes ) {
		return std::memcpy( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* CopyMemoryNonTemporal( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, const size_t lengthInBytes ) {
		return std::memcpy( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T* CopyArray( T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, const size_t arraySizeInElements ) {
		return static_cast<T*>(Eldritch2::CopyMemory( static_cast<void*>(destination), static_cast<const void*>(source), arraySizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETPureFunctionHint auto CopyArray( T (&destinationArray)[arraySizeInElements], const T (&sourceArray)[arraySizeInElements] ) -> decltype(destinationArray) {
		return (Eldritch2::CopyArray( destinationArray, sourceArray, arraySizeInElements ), destinationArray);
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T* CopyArrayNonTemporal( T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, const size_t arraySizeInElements ) {
		return static_cast<T*>(Eldritch2::CopyMemoryNonTemporal( static_cast<void*>(destination), static_cast<const void*>(source), arraySizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETPureFunctionHint auto CopyArrayNonTemporal( T (&destination)[arraySizeInElements], const T (&source)[arraySizeInElements] ) -> decltype(destination) {
		return (Eldritch2::CopyArrayNonTemporal( destination, source, arraySizeInElements ), destination);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* MoveMemory( void* destinationBuffer, const void* sourceBuffer, const size_t lengthInBytes ) {
		return std::memmove( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* MoveMemoryNonTemporal( void* destinationBuffer, const void* sourceBuffer, const size_t lengthInBytes ) {
		return Eldritch2::MoveMemory( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint void* SetMemory( void* destination, int bitPattern, size_t lengthInBytes ) {
		return std::memset( destination, bitPattern, lengthInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint T* ZeroMemory( T* destination, const size_t arraySizeInElements ) {
		static_assert( std::is_trivially_copyable<T>::value, "Zeroing polymorphic types destroys virtual function tables and is disabled." );

	// ---

		return static_cast<T*>(Eldritch2::SetMemory( destination, 0, arraySizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

//	Semi-hack: Define a specialization for the void type for compatibility with the more conventional usage
	template <>
	ETForceInlineHint ETPureFunctionHint void* ZeroMemory<void>( void* destination, const size_t bufferSizeInBytes ) {
		return Eldritch2::SetMemory( destination, 0, bufferSizeInBytes );
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETPureFunctionHint auto ZeroMemory( T (&destination)[arraySizeInElements] ) -> decltype(destination) {
		return (Eldritch2::ZeroMemory( static_cast<T*>(destination), arraySizeInElements ), destination);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareMemory( const void* buffer0, const void* buffer1, const size_t spanInBytes ) {
		return std::memcmp( buffer0, buffer1, spanInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareMemoryCaseInsensitive( const void* buffer0, const void* buffer1, const size_t spanInBytes ) {
		return _memicmp( buffer0, buffer1, spanInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint int CompareArray( const T* array0, const T* array1, const size_t arraySizeInElements ) {
		return Eldritch2::CompareMemory( static_cast<const void*>(array0), static_cast<const void*>(array1), arraySizeInElements * sizeof(T) );
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETPureFunctionHint int CompareArray( const T( &array0 )[arraySizeInElements], const T( &array1 )[arraySizeInElements] ) {
		return Eldritch2::CompareArray( static_cast<const T*>(array0), static_cast<const T*>(array1), arraySizeInElements );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareString( const char* string0, const char* string1 ) {
		return std::strcmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareString( const wchar_t* string0, const wchar_t* string1 ) {
		return std::wcscmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareString( const char* string0, const char* string1, const size_t lengthInCharacters ) {
		return std::strncmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareString( const wchar_t* string0, const wchar_t* string1, const size_t lengthInCharacters ) {
		return std::wcsncmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint int CompareString( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] ) {
		return Eldritch2::CompareString( static_cast<const char*>(string0), static_cast<const char*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint int CompareString( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] ) {
		return Eldritch2::CompareString( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareStringCaseInsensitive( const char* string0, const char* string1 ) {
		return _stricmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint int CompareStringCaseInsensitive( const wchar_t* string0, const wchar_t* string1 ) {
		return _wcsicmp( string0, string1 );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint int CompareStringCaseInsensitive( const char( &string0 )[stringSizeInCharacters], const char( &string1 )[stringSizeInCharacters] ) {
		return std::_memicmp( string0, string1, stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint int CompareStringCaseInsensitive( const wchar_t( &string0 )[stringSizeInCharacters], const wchar_t( &string1 )[stringSizeInCharacters] ) {
		return Eldritch2::CompareStringCaseInsensitive( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1) );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareString( const char* string0, const char* string1 ) {
		return 0 == Eldritch2::CompareString( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareString( const wchar_t* string0, const wchar_t* string1 ) {
		return 0 == Eldritch2::CompareString( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareString( const char* string0, const char* string1, const size_t lengthInCharacters ) {
		return 0 == Eldritch2::CompareString( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareString( const wchar_t* string0, const wchar_t* string1, const size_t lengthInCharacters ) {
		return 0 == Eldritch2::CompareString( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint bool EqualityCompareString( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] ) {
		return Eldritch2::EqualityCompareString( static_cast<const char*>(string0), static_cast<const char*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint bool EqualityCompareString( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] ) {
		return Eldritch2::EqualityCompareString( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareStringCaseInsensitive( const char* string0, const char* string1 ) {
		return 0 == Eldritch2::CompareStringCaseInsensitive( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareStringCaseInsensitive( const wchar_t* string0, const wchar_t* string1 ) {
		return 0 == Eldritch2::CompareStringCaseInsensitive( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareStringCaseInsensitive( const char* string0, const char* string1, const size_t lengthInCharacters ) {
		return 0 == Eldritch2::CompareStringCaseInsensitive( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint bool EqualityCompareStringCaseInsensitive( const wchar_t* string0, const wchar_t* string1, const size_t lengthInCharacters ) {
		return 0 == Eldritch2::CompareStringCaseInsensitive( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint bool EqualityCompareStringCaseInsensitive( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] ) {
		return Eldritch2::EqualityCompareStringCaseInsensitive( static_cast<const char*>(string0), static_cast<const char*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint bool EqualityCompareStringCaseInsensitive( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] ) {
		return Eldritch2::EqualityCompareStringCaseInsensitive( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), stringSizeInCharacters );
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

	template<size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint size_t StringLength( const char (&string)[stringSizeInCharacters] ) {
		return Eldritch2::StringLength( static_cast<const char*>(string) );
	}

// ---------------------------------------------------

	template<size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint size_t StringLength( const wchar_t (&string)[stringSizeInCharacters] ) {
		return Eldritch2::StringLength( static_cast<const wchar_t*>(string) );
	}

// ---------------------------------------------------


#if( ET_COMPILER_IS_MSVC )
//	MSVC yells at us for using the insecure string functions here.
//	Hush it up, as we try to be as secure as possible w/ the template mechanism.
#	pragma warning( push )
#	pragma warning( disable : 4996 )
#endif

	ETForceInlineHint ETPureFunctionHint char* CopyString( char* destinationString, const char* sourceString, const size_t maxLengthInCharacters ) {
		return std::strncpy( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* CopyString( wchar_t* destinationString, const wchar_t* sourceString, const size_t maxLengthInCharacters ) {
		return std::wcsncpy( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint auto CopyString( char (&destinationString)[stringSizeInCharacters], const char* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::CopyString( static_cast<char*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint auto CopyString( wchar_t (&destinationString)[stringSizeInCharacters], const wchar_t* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::CopyString( static_cast<wchar_t*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* AppendString( char* destinationString, const char* sourceString, const size_t maxLengthInCharacters ) {
		return std::strncat( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* AppendString( wchar_t* destinationString, const wchar_t* sourceString, const size_t maxLengthInCharacters ) {
		return std::wcsncat( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint auto AppendString( char (&destinationString)[stringSizeInCharacters], const char* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::AppendString( static_cast<char*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETPureFunctionHint auto AppendString( wchar_t( &destinationString )[stringSizeInCharacters], const wchar_t* sourceString ) -> decltype(destinationString) {
		return (Eldritch2::AppendString( static_cast<wchar_t*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
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
		if( !substring || ('\0' == substring[0]) ) {
			return nullptr;
		}

		const auto	substringLength( Eldritch2::StringLength( substring ) );
		const auto	stringLength( Eldritch2::StringLength( string ) );
		
		if( stringLength < substringLength ) {
			return nullptr;
		}	

		for( const auto* cursor( string + stringLength - substringLength ); cursor >= string; --cursor ) {
			if( Eldritch2::EqualityCompareString( cursor, substring, substringLength ) ) {
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
		if( !substring || (L'\0' == substring[0]) ) {
			return nullptr;
		}

		const auto	substringLength( Eldritch2::StringLength( substring ) );
		const auto	stringLength( Eldritch2::StringLength( string ) );

		if( stringLength < substringLength ) {
			return nullptr;
		}

		for( const auto* cursor( string + stringLength - substringLength ); cursor >= string; --cursor ) {
			if( Eldritch2::EqualityCompareString( cursor, substring, substringLength ) ) {
				return cursor;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint char* FindEndOfString( char* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* FindEndOfString( const char* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint wchar_t* FindEndOfString( wchar_t* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* FindEndOfString( const wchar_t* string ) {
		return string + Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const char* StripLeadingWhitespace( const char* string, const char* const stringEnd ) {
		for( ; string != stringEnd && (0 != std::isspace( static_cast<int>(*string) )); ++string ) {}

		return string;
	}

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* StripLeadingWhitespace( const wchar_t* string, const wchar_t* const stringEnd ) {
		for( ; string != stringEnd && (0 != std::iswspace( static_cast<::wint_t>(*string) )); ++string ) {}

		return string;
	}

// ---------------------------------------------------

#if( ET_COMPILER_IS_MSVC )
//	MSVC complains needlessly about a harmless type promotion for std::isspace().
#	pragma warning( push )
#	pragma warning( disable : 6330 )
#endif
	ETForceInlineHint ETPureFunctionHint const char* StripTrailingWhitespace( const char* const string, const char* stringEnd ) {
		for( ; string != stringEnd && std::isspace( stringEnd[-1] ); --stringEnd ) {}

		return stringEnd;
	}
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint const wchar_t* StripTrailingWhitespace( const wchar_t* const string, const wchar_t* stringEnd ) {
		for( ; string != stringEnd && std::iswspace( stringEnd[-1] ); --stringEnd ) {}

		return stringEnd;
	}

// ---------------------------------------------------

	template <typename Character, size_t stringSizeInCharacters>
	ETPureFunctionHint auto PrintFormatted( Character (&destinationString)[stringSizeInCharacters], const Character* const formatString, ... ) -> decltype(destinationString) {
		va_list	args;

		va_start( args, formatString );
			Eldritch2::PrintFormatted( destinationString, stringSizeInCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	template <typename Character, size_t stringSizeInCharacters>
	ETPureFunctionHint auto PrintFormatted( Character (&destinationString)[stringSizeInCharacters], const Character* const formatString, va_list args ) -> decltype(destinationString) {
		va_list forwardedArgs;
		
		va_copy( forwardedArgs, args );
			Eldritch2::PrintFormatted( destinationString, stringSizeInCharacters, formatString, args );
		va_end( forwardedArgs );

		return destinationString;
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint const T* SeekArrayDifference( T* ETRestrictPtrHint array0, T* ETRestrictPtrHint array1, const size_t arraySizeInElements ) {
		for( T* ETRestrictPtrHint const end( array0 + arraySizeInElements ); array0 != end; ++array0, ++array1 ) {
			if( *array0 != *array1 ) {
				return array0;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETPureFunctionHint const T* SeekArrayDifference( const T* ETRestrictPtrHint array0, const T* ETRestrictPtrHint array1, const size_t arraySizeInElements ) {
		for( const T* ETRestrictPtrHint const end( array0 + arraySizeInElements ); array0 != end; ++array0, ++array1 ) {
			if( *array0 != *array1 ) {
				return array0;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETPureFunctionHint T* SeekArrayDifference( T (&array0)[arraySizeInElements], T (&array1)[arraySizeInElements] ) {
		return Eldritch2::SeekArrayDifference( static_cast<T*>(array0), static_cast<T*>(array1), arraySizeInElements );
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETPureFunctionHint const T* SeekArrayDifference( const T(&array0)[arraySizeInElements], const T(&array1)[arraySizeInElements] ) {
		return Eldritch2::SeekArrayDifference( static_cast<const T*>(array0), static_cast<const T*>(array1), arraySizeInElements );
	}

}	// namespace Eldritch2