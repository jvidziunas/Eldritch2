/*==================================================================*\
  Prefetch.Win.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2011 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
#include <xmmintrin.h>
#include <emmintrin.h>
//------------------------------------------------------------------//

namespace Eldritch2Detail {

	template <size_t N>
	struct BlockFetchSizeCalculator { 
		ENUM_CLASS( size_t ) { 
			OFFSET = ( 64 * (N-1) )
		};
	};

// ---------------------------------------------------

	template <size_t N>
	ETForceInlineHint void L0BlockFetchHelper( const void* address ) {
		BlockFetchHelper<N-1>( address );
		::_mm_prefetch( static_cast<const char*>( address ) + BlockFetchSizeCalculator<N>::OFFSET, _MM_HINT_T0 );
	}

// ---------------------------------------------------

	template <>
	ETForceInlineHint void L0BlockFetchHelper<0>( const void* ) {}

// ---------------------------------------------------

	template <size_t N>
	ETForceInlineHint void L1BlockFetchHelper( const void* address ) {
		BlockFetchHelper<N-1>( address );
		::_mm_prefetch( static_cast<const char*>( address ) + BlockFetchSizeCalculator<N>::OFFSET, _MM_HINT_T1 );
	}

// ---------------------------------------------------

	template <>
	ETForceInlineHint void L1BlockFetchHelper<0>( const void* ) {}

// ---------------------------------------------------

	template <size_t N>
	ETForceInlineHint void L2BlockFetchHelper( const void* address ) {
		BlockFetchHelper<N-1>( address );
		::_mm_prefetch( static_cast<const char*>( address ) + BlockFetchSizeCalculator<N>::OFFSET, _MM_HINT_T2 );
	}

// ---------------------------------------------------

	template <>
	ETForceInlineHint void L2BlockFetchHelper<0>( const void* ) {}

// ---------------------------------------------------

	template <size_t N>
	ETForceInlineHint void BlockWriteHelper( void* const destination, const void* source ) {
		BlockWriteHelper<N-1>( destination, source );
		::_mm_stream_si32( static_cast<int*>( destination ) + (N-1), *(static_cast<const int*>( source ) + (N-1)) );
	}

// ---------------------------------------------------

	template <>
	ETForceInlineHint void BlockWriteHelper<0>( void* const, const void* ) {}

}	// namespace Eldritch2Detail

namespace Eldritch2 {

	template<typename T>
	ETForceInlineHint void Level0Prefetch( const T& data ) {
		::Eldritch2Detail::L0BlockFetchHelper<sizeof(T)/64>( static_cast<const void*>(&data) );
	}

// ---------------------------------------------------

	template<typename T>
	ETForceInlineHint void Level1Prefetch( const T& data ) {
		::Eldritch2Detail::L1BlockFetchHelper<sizeof(T)/64>( static_cast<const void*>(&data) );
	}

// ---------------------------------------------------

	template<typename T>
	ETForceInlineHint void Level2Prefetch( const T& data ) {
		::Eldritch2Detail::L2BlockFetchHelper<sizeof(T)/64>( static_cast<const void*>(&data) );
	}

// ---------------------------------------------------

	template<typename T>
	ETForceInlineHint void NonTemporalWrite( T& destination, const T& data ) {
		ETCompileTimeAssert( 0 == (sizeof(T) % sizeof(int)) );

		::Eldritch2Detail::BlockWriteHelper<sizeof(T)/sizeof(int)>( static_cast<void*>(&destination), static_cast<const void*>(&data) );
	}

}	// namespace Eldritch2