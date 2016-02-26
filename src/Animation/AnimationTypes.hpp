/*==================================================================*\
  AnimationTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Quaternion;
}

namespace Eldritch2 {
namespace Animation {

	using BoneIndex = ::Eldritch2::int16;
	using KeyTime	= ::Eldritch2::float16;

// ---------------------------------------------------

	struct CompressedUnitQuaternion {
		operator ::Eldritch2::Quaternion() const;

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::uint32	reconstructedComponentIndex : 2;
		::Eldritch2::uint32	component0					: 10;
		::Eldritch2::uint32	component1					: 10;
		::Eldritch2::uint32	component2					: 10;
	};

}	// namespace Animation
}	// namespace Eldritch2