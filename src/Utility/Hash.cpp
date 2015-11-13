/*==================================================================*\
  Hash.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/Hash.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	static ETForceInlineHint ETNoAliasHint uint64 mix( uint64& value ) {
		value ^= value >> 23;
		value *= 0x2127599bf4325c37ULL;
		value ^= value >> 47;

		return value;
	}

// ---------------------------------------------------

	ETNoAliasHint uint64 FastHash( const void* bufferToHash, const size_t lengthInBytes, const uint64 seed ) {
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
			case 7: v ^= (uint64)pos2[6] << 48;
			case 6: v ^= (uint64)pos2[5] << 40;
			case 5: v ^= (uint64)pos2[4] << 32;
			case 4: v ^= (uint64)pos2[3] << 24;
			case 3: v ^= (uint64)pos2[2] << 16;
			case 2: v ^= (uint64)pos2[1] << 8;
			case 1: v ^= (uint64)pos2[0];
				hash ^= mix( v );
				hash *= m;
		}	// switch( lengthInBytes & 7 )

		return mix( hash );
	}

// ---------------------------------------------------

	ETNoAliasHint uint32 FastHash( const void* bufferToHash, const size_t lengthInBytes, const uint32 seed ) {
		// the following trick converts the 64-bit hashcode to Fermat
		// residue, which shall retain information from both the higher
		// and lower parts of hashcode.
		const uint64	h( FastHash( bufferToHash, lengthInBytes, static_cast<uint64>(seed) ) );

		return static_cast<uint32>(h - (h >> 32));
	}

}	// anonymous namespace

namespace Eldritch2 {
namespace Detail {

	ETNoAliasHint size_t HashBase::operator()( const void* sourceData, const size_t sourceDataSizeInBytes, const size_t seed ) const {
		return FastHash( sourceData, sourceDataSizeInBytes, seed );
	}

}	// namespace Detail
}	// namespace Eldritch2

