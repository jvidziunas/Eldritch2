/*==================================================================*\
  SpatialHash.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/SpatialHash.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	SpatialHasher::SpatialHasher( const float32 cellLengthInMeters, const float32 cellHeightInMeters ) : _inverseResolution( ::Eldritch2::Reciprocal( cellLengthInMeters ),
																															 ::Eldritch2::Reciprocal( cellHeightInMeters ),
																															 ::Eldritch2::Reciprocal( cellLengthInMeters ),
																															 1.0f ),
																										 _resolution( cellLengthInMeters,
																													  cellHeightInMeters,
																													  cellLengthInMeters,
																													  1.0f ) {}

}	// namespace Eldritch2