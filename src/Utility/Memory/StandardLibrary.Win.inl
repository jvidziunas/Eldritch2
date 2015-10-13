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
#include <cstdio>
#include <cwchar>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETForceInlineHint ETNoAliasHint void* CopyMemory( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, const size_t lengthInBytes ) {
		return ::std::memcpy( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint void* CopyMemoryNonTemporal( void* ETRestrictPtrHint destinationBuffer, const void* ETRestrictPtrHint sourceBuffer, const size_t lengthInBytes ) {
		return ::std::memcpy( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETNoAliasHint T* CopyArray( T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, const size_t arraySizeInElements ) {
		return static_cast<T*>(::Eldritch2::CopyMemory( static_cast<void*>(destination), static_cast<const void*>(source), arraySizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETNoAliasHint auto CopyArray( T (&destinationArray)[arraySizeInElements], const T (&sourceArray)[arraySizeInElements] ) -> decltype(destinationArray) {
		return (::Eldritch2::CopyArray( destinationArray, sourceArray, arraySizeInElements ), destinationArray);
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETNoAliasHint T* CopyArrayNonTemporal( T* ETRestrictPtrHint destination, const T* ETRestrictPtrHint source, const size_t arraySizeInElements ) {
		return static_cast<T*>(::Eldritch2::CopyMemoryNonTemporal( static_cast<void*>(destination), static_cast<const void*>(source), arraySizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETNoAliasHint auto CopyArrayNonTemporal( T (&destination)[arraySizeInElements], const T (&source)[arraySizeInElements] ) -> decltype(destination) {
		return (::Eldritch2::CopyArrayNonTemporal( destination, source, arraySizeInElements ), destination);
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint void* MoveMemory( void* destinationBuffer, const void* sourceBuffer, const size_t lengthInBytes ) {
		return ::std::memmove( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint void* MoveMemoryNonTemporal( void* destinationBuffer, const void* sourceBuffer, const size_t lengthInBytes ) {
		return ::Eldritch2::MoveMemory( destinationBuffer, sourceBuffer, lengthInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint void* SetMemory( void* destination, int bitPattern, size_t lengthInBytes ) {
		return ::std::memset( destination, bitPattern, lengthInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETNoAliasHint T* ZeroMemory( T* destination, const size_t arraySizeInElements ) {
		static_assert( ::std::is_trivially_copyable<T>::value, "Zeroing polymorphic types destroys virtual function tables and is disabled." );

	// ---

		return static_cast<T*>(::Eldritch2::SetMemory( destination, 0, arraySizeInElements * sizeof(T) ));
	}

// ---------------------------------------------------

	// Semi-hack: Define a specialization for the void type for compatibility with the more conventional usage
	template <>
	ETForceInlineHint ETNoAliasHint void* ZeroMemory<void>( void* destination, const size_t bufferSizeInBytes ) {
		return ::Eldritch2::SetMemory( destination, 0, bufferSizeInBytes );
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETNoAliasHint auto ZeroMemory( T (&destination)[arraySizeInElements] ) -> decltype(destination) {
		return (::Eldritch2::ZeroMemory( static_cast<T*>(destination), arraySizeInElements ), destination);
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareMemory( const void* buffer0, const void* buffer1, const size_t spanInBytes ) {
		return ::std::memcmp( buffer0, buffer1, spanInBytes );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareMemoryCaseInsensitive( const void* buffer0, const void* buffer1, const size_t spanInBytes ) {
		return _memicmp( buffer0, buffer1, spanInBytes );
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETNoAliasHint int CompareArray( const T* array0, const T* array1, const size_t arraySizeInElements ) {
		return ::Eldritch2::CompareMemory( static_cast<const void*>(array0), static_cast<const void*>(array1), arraySizeInElements * sizeof(T) );
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETNoAliasHint int CompareArray( const T( &array0 )[arraySizeInElements], const T( &array1 )[arraySizeInElements] ) {
		return ::Eldritch2::CompareArray( static_cast<const T*>(array0), static_cast<const T*>(array1), arraySizeInElements );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareString( const char* string0, const char* string1 ) {
		return ::std::strcmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareString( const wchar_t* string0, const wchar_t* string1 ) {
		return ::std::wcscmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareString( const char* string0, const char* string1, const size_t lengthInCharacters ) {
		return ::std::strncmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareString( const wchar_t* string0, const wchar_t* string1, const size_t lengthInCharacters ) {
		return ::std::wcsncmp( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint int CompareString( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] ) {
		return ::Eldritch2::CompareString( static_cast<const char*>(string0), static_cast<const char*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint int CompareString( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] ) {
		return ::Eldritch2::CompareString( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareStringCaseInsensitive( const char* string0, const char* string1 ) {
		return _stricmp( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint int CompareStringCaseInsensitive( const wchar_t* string0, const wchar_t* string1 ) {
		return _wcsicmp( string0, string1 );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint int CompareStringCaseInsensitive( const char( &string0 )[stringSizeInCharacters], const char( &string1 )[stringSizeInCharacters] ) {
		return ::std::_memicmp( string0, string1, stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint int CompareStringCaseInsensitive( const wchar_t( &string0 )[stringSizeInCharacters], const wchar_t( &string1 )[stringSizeInCharacters] ) {
		return ::Eldritch2::CompareStringCaseInsensitive( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1) );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareString( const char* string0, const char* string1 ) {
		return 0 == ::Eldritch2::CompareString( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareString( const wchar_t* string0, const wchar_t* string1 ) {
		return 0 == ::Eldritch2::CompareString( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareString( const char* string0, const char* string1, const size_t lengthInCharacters ) {
		return 0 == ::Eldritch2::CompareString( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareString( const wchar_t* string0, const wchar_t* string1, const size_t lengthInCharacters ) {
		return 0 == ::Eldritch2::CompareString( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint bool EqualityCompareString( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] ) {
		return ::Eldritch2::EqualityCompareString( static_cast<const char*>(string0), static_cast<const char*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint bool EqualityCompareString( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] ) {
		return ::Eldritch2::EqualityCompareString( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareStringCaseInsensitive( const char* string0, const char* string1 ) {
		return 0 == ::Eldritch2::CompareStringCaseInsensitive( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareStringCaseInsensitive( const wchar_t* string0, const wchar_t* string1 ) {
		return 0 == ::Eldritch2::CompareStringCaseInsensitive( string0, string1 );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareStringCaseInsensitive( const char* string0, const char* string1, const size_t lengthInCharacters ) {
		return 0 == ::Eldritch2::CompareStringCaseInsensitive( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint bool EqualityCompareStringCaseInsensitive( const wchar_t* string0, const wchar_t* string1, const size_t lengthInCharacters ) {
		return 0 == ::Eldritch2::CompareStringCaseInsensitive( string0, string1, lengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint bool EqualityCompareStringCaseInsensitive( const char (&string0)[stringSizeInCharacters], const char (&string1)[stringSizeInCharacters] ) {
		return ::Eldritch2::EqualityCompareStringCaseInsensitive( static_cast<const char*>(string0), static_cast<const char*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint bool EqualityCompareStringCaseInsensitive( const wchar_t (&string0)[stringSizeInCharacters], const wchar_t (&string1)[stringSizeInCharacters] ) {
		return ::Eldritch2::EqualityCompareStringCaseInsensitive( static_cast<const wchar_t*>(string0), static_cast<const wchar_t*>(string1), stringSizeInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint size_t StringLength( const char* string ) {
		return ::std::strlen( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint size_t StringLength( const wchar_t* string ) {
		return ::std::wcslen( string );
	}

// ---------------------------------------------------

	template<size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint size_t StringLength( const char (&string)[stringSizeInCharacters] ) {
		return ::Eldritch2::StringLength( static_cast<const char*>(string) );
	}

// ---------------------------------------------------

	template<size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint size_t StringLength( const wchar_t (&string)[stringSizeInCharacters] ) {
		return ::Eldritch2::StringLength( static_cast<const wchar_t*>(string) );
	}

// ---------------------------------------------------

	// MSVC yells at us for using the insecure string functions here.
	// Hush it up, as we try to be as secure as possible w/ the template mechanism.
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( push )
#		pragma warning( disable : 4996 )
#endif

	ETForceInlineHint ETNoAliasHint char* CopyString( char* destinationString, const char* sourceString, const size_t maxLengthInCharacters ) {
		return ::std::strncpy( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint wchar_t* CopyString( wchar_t* destinationString, const wchar_t* sourceString, const size_t maxLengthInCharacters ) {
		return ::std::wcsncpy( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint auto CopyString( char (&destinationString)[stringSizeInCharacters], const char* sourceString ) -> decltype(destinationString) {
		return (::Eldritch2::CopyString( static_cast<char*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint auto CopyString( wchar_t (&destinationString)[stringSizeInCharacters], const wchar_t* sourceString ) -> decltype(destinationString) {
		return (::Eldritch2::CopyString( static_cast<wchar_t*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint char* AppendString( char* destinationString, const char* sourceString, const size_t maxLengthInCharacters ) {
		return ::std::strncat( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint wchar_t* AppendString( wchar_t* destinationString, const wchar_t* sourceString, const size_t maxLengthInCharacters ) {
		return ::std::wcsncat( destinationString, sourceString, maxLengthInCharacters );
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint auto AppendString( char (&destinationString)[stringSizeInCharacters], const char* sourceString ) -> decltype(destinationString) {
		return (::Eldritch2::AppendString( static_cast<char*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint auto AppendString( wchar_t( &destinationString )[stringSizeInCharacters], const wchar_t* sourceString ) -> decltype(destinationString) {
		return (::Eldritch2::AppendString( static_cast<wchar_t*>(destinationString), sourceString, stringSizeInCharacters ), destinationString);
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint char* FindFirstInstance( char* string, const char character ) {
		return ::std::strchr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint const char* FindFirstInstance( const char* string, const char character ) {
		return ::std::strchr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint char* FindFirstInstance( char* string, const char* substring ) {
		return ::std::strstr( string, substring );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint const char* FindFirstInstance( const char* string, const char* substring ) {
		return ::std::strstr( string, substring );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint wchar_t* FindFirstInstance( wchar_t* string, const wchar_t character ) {
		return ::std::wcschr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint const wchar_t* FindFirstInstance( const wchar_t* string, const wchar_t character ) {
		return ::std::wcschr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint wchar_t* FindFirstInstance( wchar_t* string, const wchar_t* substring ) {
		return ::std::wcsstr( string, substring );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint const wchar_t* FindFirstInstance( const wchar_t* string, const wchar_t* substring ) {
		return ::std::wcsstr( string, substring );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint char* FindLastInstance( char* string, const char character ) {
		return ::std::strrchr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint const char* FindLastInstance( const char* string, const char character ) {
		return ::std::strrchr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint char* FindLastInstance( char* string, const char* substring ) {
		return const_cast<char*>(::Eldritch2::FindFirstInstance( const_cast<const char*>(string), substring ));
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint const char* FindLastInstance( const char* string, const char* substring ) {
		if( !substring || ('\0' == substring[0]) ) {
			return nullptr;
		}

		const auto	substringLength( ::Eldritch2::StringLength( substring ) );
		const auto	stringLength( ::Eldritch2::StringLength( string ) );
		
		if( stringLength < substringLength ) {
			return nullptr;
		}	

		for( const auto* cursor( string + stringLength - substringLength ); cursor >= string; --cursor ) {
			if( ::Eldritch2::EqualityCompareString( cursor, substring, substringLength ) ) {
				return cursor;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint wchar_t* FindLastInstance( wchar_t* string, const wchar_t character ) {
		return ::std::wcsrchr( string, character );
	}

// ---------------------------------------------------
	
	ETForceInlineHint ETNoAliasHint const wchar_t* FindLastInstance( const wchar_t* string, const wchar_t character ) {
		return ::std::wcsrchr( string, character );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint wchar_t* FindLastInstance( wchar_t* string, const wchar_t* substring ) {
		return const_cast<wchar_t*>(::Eldritch2::FindFirstInstance( const_cast<const wchar_t*>(string), substring ));
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint const wchar_t* FindLastInstance( const wchar_t* string, const wchar_t* substring ) {
		if( !substring || (L'\0' == substring[0]) ) {
			return nullptr;
		}

		const auto	substringLength( ::Eldritch2::StringLength( substring ) );
		const auto	stringLength( ::Eldritch2::StringLength( string ) );

		if( stringLength < substringLength ) {
			return nullptr;
		}

		for( const auto* cursor( string + stringLength - substringLength ); cursor >= string; --cursor ) {
			if( ::Eldritch2::EqualityCompareString( cursor, substring, substringLength ) ) {
				return cursor;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint char* FindEndOfString( char* string ) {
		return string + ::Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint const char* FindEndOfString( const char* string ) {
		return string + ::Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint wchar_t* FindEndOfString( wchar_t* string ) {
		return string + ::Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint const wchar_t* FindEndOfString( const wchar_t* string ) {
		return string + ::Eldritch2::StringLength( string );
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint char* PrintFormatted( char* destinationString, size_t maxCharacters, const char* formatString, va_list args ) {
#		if( !ET_COMPILER_IS_MSVC )
			va_list	argsCopy;

			va_copy( argsCopy, args )
			_vsnprintf( destinationString, maxCharacters, formatString, argsCopy );
#		else
			_vsnprintf( destinationString, maxCharacters, formatString, args );
#		endif

		return destinationString;
	}

// ---------------------------------------------------

	ETForceInlineHint ETNoAliasHint wchar_t* PrintFormatted( wchar_t* destinationString, size_t maxCharacters, const wchar_t* formatString, va_list args ) {
#		if( !ET_COMPILER_IS_MSVC )
			va_list argsCopy;

			va_copy( argsCopy, args )
			_vsnwprintf( destinationString, maxCharacters, formatString, argsCopy );
#		else
			_vsnwprintf( destinationString, maxCharacters, formatString, args );
#		endif

		return destinationString;
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint auto PrintFormatted( char (&destinationString)[stringSizeInCharacters], const char* formatString, va_list args ) -> decltype(destinationString) {
#		if( !ET_COMPILER_IS_MSVC )
			va_list argsCopy;

			va_copy( argsCopy, args )
			vsnprintf( destinationString, stringSizeInCharacters, formatString, argsCopy );
#		else
			vsnprintf( destinationString, stringSizeInCharacters, formatString, args );
#		endif

		return destinationString;
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETForceInlineHint ETNoAliasHint auto PrintFormatted( wchar_t (&destinationString)[stringSizeInCharacters], const wchar_t* formatString, va_list args ) -> decltype(destinationString) {
#		if( !ET_COMPILER_IS_MSVC )
			va_list argsCopy;

			va_copy( argsCopy, args )
			_vsnwprintf( destinationString, stringSizeInCharacters, formatString, argsCopy );
#		else
			_vsnwprintf( destinationString, stringSizeInCharacters, formatString, args );
#		endif

		return destinationString;
	}

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETNoAliasHint const T* SeekArrayDifference( T* ETRestrictPtrHint array0, T* ETRestrictPtrHint array1, const size_t arraySizeInElements ) {
		for( T* ETRestrictPtrHint const end( array0 + arraySizeInElements ); array0 != end; ++array0, ++array1 ) {
			if( *array0 != *array1 ) {
				return array0;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	template <typename T>
	ETForceInlineHint ETNoAliasHint const T* SeekArrayDifference( const T* ETRestrictPtrHint array0, const T* ETRestrictPtrHint array1, const size_t arraySizeInElements ) {
		for( const T* ETRestrictPtrHint const end( array0 + arraySizeInElements ); array0 != end; ++array0, ++array1 ) {
			if( *array0 != *array1 ) {
				return array0;
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETNoAliasHint T* SeekArrayDifference( T (&array0)[arraySizeInElements], T (&array1)[arraySizeInElements] ) {
		return ::Eldritch2::SeekArrayDifference( static_cast<T*>(array0), static_cast<T*>(array1), arraySizeInElements );
	}

// ---------------------------------------------------

	template <typename T, size_t arraySizeInElements>
	ETForceInlineHint ETNoAliasHint const T* SeekArrayDifference( const T(&array0)[arraySizeInElements], const T(&array1)[arraySizeInElements] ) {
		return ::Eldritch2::SeekArrayDifference( static_cast<const T*>(array0), static_cast<const T*>(array1), arraySizeInElements );
	}

}	// namespace Eldritch2