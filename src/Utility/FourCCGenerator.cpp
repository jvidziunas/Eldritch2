/*==================================================================*\
  FourCCGenerator.cpp
  ------------------------------------------------------------------
  Purpose:
  Utility object that can be used to generate FourCC codes used to
  identify data at compile time.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/FourCCGenerator.hpp>
#include <Utility/Memory/MemStdLib.hpp>
#include <algorithm>
#include <cctype>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

	uint32 CStringToFourCC( const char* src ) {
		union {
			char	bytes[sizeof(uint32)];
			uint32	fourCC;
		};

		ZeroMemory( bytes );

		for( size_t i( 0u ); (*src && (i < 4)); ++i ) {
			bytes[i] = static_cast<char>(toupper( *src++ )); 
		}

		return fourCC;
	}

// ---------------------------------------------------

	uint32 CStringToFourCC( const wchar_t* src ) {
		union {
			char	bytes[sizeof(uint32)];
			uint32	fourCC;
		};

		ZeroMemory( bytes );

		for( size_t i( 0u ); (*src && (i < 4)); ++i ) {
			bytes[i] = static_cast<char>(toupper( *src++ )); 
		}

		return fourCC;
	}

}	// namespace Utility
}	// namespace Eldritch2