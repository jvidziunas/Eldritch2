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
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	static ETForceInlineHint ETPureFunctionHint uint64 mix( uint64& value ) {
		value ^= value >> 23;
		value *= 0x2127599bf4325c37ULL;
		value ^= value >> 47;

		return value;
	}

// ---------------------------------------------------

	static ETPureFunctionHint uint64 FastHash( const void* bufferToHash, size_t lengthInBytes, uint64 seed ) {
		enum : uint64 {
			m = 0x880355f21e6d1965ull
		};

	// ---

		auto		pos( static_cast<const uint64*>(bufferToHash) );
		uint64		hash( seed ^ (lengthInBytes * m) );
		uint64		v;

		for( const auto end( pos + (lengthInBytes / sizeof(pos[0])) ); pos != end; ) {
			v		= *pos++;
			hash	^= mix( v );
			hash	*= m;
		}

		const auto	pos2( reinterpret_cast<const unsigned char*>(pos) );

		v = 0ull;

		switch( lengthInBytes & 7 ) {
			case 7: v ^= static_cast<uint64>(pos2[6]) << 48;
			case 6: v ^= static_cast<uint64>(pos2[5]) << 40;
			case 5: v ^= static_cast<uint64>(pos2[4]) << 32;
			case 4: v ^= static_cast<uint64>(pos2[3]) << 24;
			case 3: v ^= static_cast<uint64>(pos2[2]) << 16;
			case 2: v ^= static_cast<uint64>(pos2[1]) << 8;
			case 1: v ^= static_cast<uint64>(pos2[0]);
				hash ^= mix( v );
				hash *= m;
		}	// switch( lengthInBytes & 7 )

		return mix( hash );
	}

// ---------------------------------------------------

	static ETPureFunctionHint uint32 FastHash( const void* bufferToHash, size_t lengthInBytes, uint32 seed ) {
	/*	the following trick converts the 64-bit hashcode to Fermat
	 *	residue, which shall retain information from both the higher
	 *	and lower parts of hashcode. */
		const uint64	h( FastHash( bufferToHash, lengthInBytes, static_cast<uint64>(seed) ) );

		return static_cast<uint32>(h - (h >> 32));
	}

}	// anonymous namespace

	ETPureFunctionHint size_t HashMemory( const void* memory, size_t sizeInBytes, size_t seed ) {
		return FastHash( memory, sizeInBytes, seed );
	}

// ---------------------------------------------------

	ETPureFunctionHint char* PrintFormatted( char* destinationString, const size_t maxCharacters, const char* const formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, maxCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETPureFunctionHint wchar_t* PrintFormatted( wchar_t* destinationString, const size_t maxCharacters, const wchar_t* const formatString, ... ) {
		va_list args;
		va_start( args, formatString );
			PrintFormatted( destinationString, maxCharacters, formatString, args );
		va_end( args );

		return destinationString;
	}

// ---------------------------------------------------

	ETPureFunctionHint char* PrintFormatted( char* destinationString, const size_t maxCharacters, const char* const formatString, va_list args ) {
		va_list forwardedArgs;
		va_copy( forwardedArgs, args );
			std::vsnprintf( destinationString, maxCharacters, formatString, forwardedArgs );
		va_end( forwardedArgs );

		return destinationString;
	}

// ---------------------------------------------------

	ETPureFunctionHint wchar_t* PrintFormatted( wchar_t* destinationString, const size_t maxCharacters, const wchar_t* const formatString, va_list args ) {
		va_list forwardedArgs;
		va_copy( forwardedArgs, args );
			std::vswprintf( destinationString, maxCharacters, formatString, forwardedArgs );
		va_end( forwardedArgs );

		return destinationString;
	}

}	// namespace Eldritch2