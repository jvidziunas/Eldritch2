/*==================================================================*\
  RandomNumberGenerator.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/RandomNumberGenerator.hpp>
#include <Utility/Math/MathUtils.hpp>
#include <dSFMT/dSFMT.c>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {

	MersenneTwisterRandomNumberGenerator::MersenneTwisterRandomNumberGenerator( ::Eldritch2::uint32 seed ) : ::MTRand( seed ), _seed( seed ) {}

// ---------------------------------------------------

	MersenneTwisterRandomNumberGenerator::~MersenneTwisterRandomNumberGenerator() {}

// ---------------------------------------------------

	float32 MersenneTwisterRandomNumberGenerator::Generate( float32 min, float32 max ) {
		return static_cast<float32>(min + MTRand::rand( max ));
	}

// ---------------------------------------------------

	float64 MersenneTwisterRandomNumberGenerator::Generate( float64 min, float64 max ) {
		return min + MTRand::rand( max );
	}

// ---------------------------------------------------

	float32 MersenneTwisterRandomNumberGenerator::GenerateNormal( float32 mean, float32 deviation ) {
		return static_cast<float32>(MTRand::randNorm( mean, deviation ));
	}

// ---------------------------------------------------

	float64 MersenneTwisterRandomNumberGenerator::GenerateNormal( float64 mean, float64 deviation ) {
		return MTRand::randNorm( mean, deviation );
	}

// ---------------------------------------------------

	int MersenneTwisterRandomNumberGenerator::Generate( int min, int max ) {
		return min + MTRand::randInt( max );
	}

// ---------------------------------------------------

	uint32 MersenneTwisterRandomNumberGenerator::GetSeed() const {
		return _seed;
	}

// ---------------------------------------------------

	void MersenneTwisterRandomNumberGenerator::SetSeed( ::Eldritch2::uint32 seed ) {
		_seed = seed;
		MTRand::seed( seed );
	}

// ---------------------------------------------------

	DSFMersenneTwisterRandomNumberGenerator::DSFMersenneTwisterRandomNumberGenerator( uint32 seed ) : _seed( (::dsfmt_init_gen_rand( this, seed ), seed) ) {}

// ---------------------------------------------------

	DSFMersenneTwisterRandomNumberGenerator::~DSFMersenneTwisterRandomNumberGenerator() {}

// ---------------------------------------------------

	float32 DSFMersenneTwisterRandomNumberGenerator::Generate( float32 min, float32 max ) {
		return static_cast<float32>(Generate( static_cast<float64>(min), static_cast<float64>(max) ));
	}

// ---------------------------------------------------

	float64 DSFMersenneTwisterRandomNumberGenerator::Generate( float64 min, float64 max ) {
		return min + ::dsfmt_genrand_open_open( this ) * (max - min);
	}

// ---------------------------------------------------

	float32 DSFMersenneTwisterRandomNumberGenerator::GenerateNormal( float32 mean, float32 deviation ) {
		return static_cast<float32>(GenerateNormal( static_cast<float64>(mean), static_cast<float64>(deviation) ));
	}

// ---------------------------------------------------

	float64 DSFMersenneTwisterRandomNumberGenerator::GenerateNormal( float64 mean, float64 deviation ) {
		float64	x;
		float64	r;
		
		for( float64 y; ; ) {
			x = 2.0 * ::dsfmt_genrand_open_open( this ) - 1.0;
			y = 2.0 * ::dsfmt_genrand_open_open( this ) - 1.0;

			r = x * x + y * y;

			if( r >= 1.0 || r == 0.0 )
				break;
		}

		return mean + x * Sqrt( -2.0 * log(r) / r ) * deviation;
	}

// ---------------------------------------------------

	int DSFMersenneTwisterRandomNumberGenerator::Generate( int min, int max ) {
		return min + (static_cast<int>(::dsfmt_genrand_uint32( this )) % (max - min));
	}

// ---------------------------------------------------

	uint32 DSFMersenneTwisterRandomNumberGenerator::GetSeed() const {
		return _seed;
	}

// ---------------------------------------------------

	void DSFMersenneTwisterRandomNumberGenerator::SetSeed( uint32 seed ) {
		_seed = seed;
		::dsfmt_init_gen_rand( this, GetSeed() );
	}

}	// namespace Utility
}	// namespace Eldritch2