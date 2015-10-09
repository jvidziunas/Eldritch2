/*==================================================================*\
  StringOperators.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/StringOperators.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	static ETForceInlineHint ETNoAliasHint uint64 mix( uint64& value ) {
		value ^= value >> 23;
		value *= 0x2127599bf4325c37ULL;
		value ^= value >> 47;

		return value;
	}

}	// anonymous namespace

namespace Eldritch2Detail {

	uint64 FastHash( const void* bufferToHash, const size_t lengthInBytes, const uint64 seed ) {
		const uint64			m( 0x880355f21e6d1965ULL );
		const uint64*			pos( static_cast<const uint64*>(bufferToHash) );
		const unsigned char*	pos2;
		uint64					h( seed ^ (lengthInBytes * m) );
		uint64					v;

		for( const uint64* const end( pos + (lengthInBytes / sizeof(pos[0])) ); pos != end; ) {
			v = *pos++;
			h ^= mix( v );
			h *= m;
		}

		pos2 = reinterpret_cast<const unsigned char*>(pos);
		v = 0;

		switch( lengthInBytes & 7 ) {
		case 7: v ^= (uint64)pos2[6] << 48;
		case 6: v ^= (uint64)pos2[5] << 40;
		case 5: v ^= (uint64)pos2[4] << 32;
		case 4: v ^= (uint64)pos2[3] << 24;
		case 3: v ^= (uint64)pos2[2] << 16;
		case 2: v ^= (uint64)pos2[1] << 8;
		case 1: v ^= (uint64)pos2[0];
			h ^= mix( v );
			h *= m;
		}

		return mix( h );
	}

// ---------------------------------------------------

	uint32 FastHash( const void* bufferToHash, const size_t lengthInBytes, const uint32 seed ) {
		// the following trick converts the 64-bit hashcode to Fermat
		// residue, which shall retain information from both the higher
		// and lower parts of hashcode.
		const uint64	h( FastHash( bufferToHash, lengthInBytes, static_cast<uint64>(seed) ) );

		return static_cast<uint32>(h - (h >> 32));
	}

}	// namespace Eldritch2Detail
