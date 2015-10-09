/*==================================================================*\
  MemStdLib.cpp
  ------------------------------------------------------------------
  Purpose:
  Utility functions that serve as replacements for standard C bulk
  memory operations and may be faster (but are guaranteed to be
  equivalent to the compiler's at worst) than their ANSI equivalent.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/MemStdLib.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {

	ETNoAliasHint char* PrintFormatted( char* destinationString, const char* formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETNoAliasHint wchar_t* PrintFormatted( wchar_t* destinationString, const wchar_t* formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETNoAliasHint char* PrintFormatted( char* destinationString, size_t maxCharacters, const char* formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, maxCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETNoAliasHint wchar_t* PrintFormatted( wchar_t* destinationString, size_t maxCharacters, const wchar_t* formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, maxCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETNoAliasHint auto PrintFormatted( char (&destinationString)[stringSizeInCharacters], const char* formatString, ... ) -> decltype(destinationString) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, stringSizeInCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	template <size_t stringSizeInCharacters>
	ETNoAliasHint auto PrintFormatted( wchar_t (&destinationString)[stringSizeInCharacters], const wchar_t* formatString, ... ) -> decltype(destinationString) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, stringSizeInCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

}	// namespace Eldritch2