/*==================================================================*\
  FrustumUtils.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/VectorTypes.hpp>
#include <Utility/MPL/MatrixTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <boost/array.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	namespace FrustumPlanes {

		STRONG_ENUM_CLASS( FrustumPlane, ::Eldritch2::uint8 ) {
			LEFT	= 0,
			RIGHT	= 1,
			TOP		= 2,
			BOTTOM	= 3,
			NEAR	= 4,
			FAR		= 5
		};

	}	// namespace FrustumPlanes

	typedef Utility::FrustumPlanes::FrustumPlane	FrustumPlane;

// ---------------------------------------------------

	typedef ::boost::array<::Eldritch2::Float4, 6>	FrustumPlaneCollection;
	typedef ::boost::array<::Eldritch2::Float4, 8>	StructureOfArraysFrustumPlaneCollection;

// ---------------------------------------------------

	ETNoThrowHint ETNoAliasHint Utility::FrustumPlaneCollection						ExtractFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

	ETNoThrowHint ETNoAliasHint Utility::FrustumPlaneCollection						ExtractNormalizedFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

	ETNoThrowHint ETNoAliasHint Utility::StructureOfArraysFrustumPlaneCollection	ExtractStructureOfArraysFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

	ETNoThrowHint ETNoAliasHint Utility::StructureOfArraysFrustumPlaneCollection	ExtractStructureOfArraysNormalizedFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

}	// namespace Utility
}	// namespace Eldritch2