/*==================================================================*\
  RandomNumberGenerator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/NonCopyable.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <Mersenne-1.1/MersenneTwister.h>
#include <dSFMT/dSFMT.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class MersenneTwisterRandomNumberGenerator : private ::MTRand {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this MersenneTwisterRandomNumberGenerator instance.
		MersenneTwisterRandomNumberGenerator( ::Eldritch2::uint32 seed );

		// Destroys this MersenneTwisterRandomNumberGenerator instance.
		~MersenneTwisterRandomNumberGenerator();

	// - PSEUDORANDOM NUMBER GENERATION ------------------

		::Eldritch2::float32	Generate( ::Eldritch2::float32 min, ::Eldritch2::float32 max );
		::Eldritch2::float64	Generate( ::Eldritch2::float64 min, ::Eldritch2::float64 max );

		::Eldritch2::float32	GenerateNormal( ::Eldritch2::float32 mean, ::Eldritch2::float32 deviation );
		::Eldritch2::float64	GenerateNormal( ::Eldritch2::float64 mean, ::Eldritch2::float64 deviation );

		int						Generate( int min, int max );

	// ---------------------------------------------------

		::Eldritch2::uint32	GetSeed() const;

		void				SetSeed( ::Eldritch2::uint32 seed );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::uint32	_seed;
	};

// ---------------------------------------------------

	class DSFMersenneTwisterRandomNumberGenerator : private ::dsfmt_t {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this DSFMersenneTwisterRandomNumberGenerator instance.
		DSFMersenneTwisterRandomNumberGenerator( ::Eldritch2::uint32 seed );

		// Destroys this DSFMersenneTwisterRandomNumberGenerator instance.
		~DSFMersenneTwisterRandomNumberGenerator();

	// - PSEUDORANDOM NUMBER GENERATION ------------------

		::Eldritch2::float32	Generate( ::Eldritch2::float32 min, ::Eldritch2::float32 max );
		::Eldritch2::float64	Generate( ::Eldritch2::float64 min, ::Eldritch2::float64 max );

		::Eldritch2::float32	GenerateNormal( ::Eldritch2::float32 mean, ::Eldritch2::float32 deviation );
		::Eldritch2::float64	GenerateNormal( ::Eldritch2::float64 mean, ::Eldritch2::float64 deviation );

		int						Generate( int min, int max );

	// ---------------------------------------------------

		::Eldritch2::uint32	GetSeed() const;

		void				SetSeed( ::Eldritch2::uint32 seed );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::uint32	_seed;
	};

}	// namespace Utility
}	// namespace Eldritch2