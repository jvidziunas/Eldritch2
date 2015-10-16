/*==================================================================*\
  StandardLibrary.cpp
  ------------------------------------------------------------------
  Purpose:
  Utility functions that serve as replacements for standard C bulk
  memory operations and may be faster (but are guaranteed to be
  equivalent to the compiler's at worst) than their ANSI equivalent.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {

	ETNoAliasHint char* PrintFormatted( char* destinationString, const size_t maxCharacters, const char* const formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, maxCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETNoAliasHint wchar_t* PrintFormatted( wchar_t* destinationString, const size_t maxCharacters, const wchar_t* const formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, maxCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETNoAliasHint char* PrintFormatted( char* destinationString, const size_t maxCharacters, const char* const formatString, va_list args ) {
		va_list forwardedArgs;
		va_copy( forwardedArgs, args );
			::std::vsnprintf( destinationString, maxCharacters, formatString, forwardedArgs );
		va_end( forwardedArgs );

		return destinationString;
	}

// ---------------------------------------------------

	ETNoAliasHint wchar_t* PrintFormatted( wchar_t* destinationString, const size_t maxCharacters, const wchar_t* const formatString, va_list args ) {
		va_list forwardedArgs;
		va_copy( forwardedArgs, args );
			::std::vswprintf( destinationString, maxCharacters, formatString, forwardedArgs );
		va_end( forwardedArgs );

		return destinationString;
	}

}	// namespace Eldritch2