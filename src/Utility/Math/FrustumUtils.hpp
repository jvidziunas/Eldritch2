/*==================================================================*\
  FrustumUtils.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
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

	enum class FrustumPlane : ::Eldritch2::uint8 {
		Left	= 0,
		Right	= 1,
		Top		= 2,
		Bottom	= 3,
		Near	= 4,
		Far		= 5
	};

// ---------------------------------------------------

	using FrustumPlaneCollection					= ::boost::array<::Eldritch2::Float4, 6>;
	using StructureOfArraysFrustumPlaneCollection	= ::boost::array<::Eldritch2::Float4, 8>;

// ---------------------------------------------------

	ETNoThrowHint ETNoAliasHint Utility::FrustumPlaneCollection						ExtractFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

	ETNoThrowHint ETNoAliasHint Utility::FrustumPlaneCollection						ExtractNormalizedFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

	ETNoThrowHint ETNoAliasHint Utility::StructureOfArraysFrustumPlaneCollection	ExtractStructureOfArraysFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

	ETNoThrowHint ETNoAliasHint Utility::StructureOfArraysFrustumPlaneCollection	ExtractStructureOfArraysNormalizedFrustumPlanes( const ::Eldritch2::Float4x4& cameraMatrix );

}	// namespace Utility
}	// namespace Eldritch2