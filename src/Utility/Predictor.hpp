/*==================================================================*\
  Predictor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{

namespace Utility
{
	
	template <typename DataType>
	class Predictor {
	public:
		typedef DataType Type;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this Predictor instance.
		Predictor( const DataType& initialValue, const DataType& initialDelta );

		// Destroys this Predictor instance.
		~Predictor();

	// ---------------------------------------------------

		bool	AddSample( ::Eldritch2::uint64 time, const DataType& value );
		bool	AddSample( ::Eldritch2::uint64 time, const DataType& value, const DataType& delta );

	// ---------------------------------------------------

		::Eldritch2::uint64	EstimateLatency() const;

	// - DATA MEMBERS ------------------------------------

	private:
		DataType	_snapPos;
		DataType	_snapVel;
		DataType	_aimPos;
		DataType	_lastPacketPos;     //  only used when re-constituting velocity
	};

}	// namespace Utility

}	// namespace Eldritch2


//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Predictor.inl>
//------------------------------------------------------------------//