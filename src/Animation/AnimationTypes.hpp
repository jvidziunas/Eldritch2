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
namespace Animation {

	namespace Animation	= ::Eldritch2::Animation;

// ---------------------------------------------------

	typedef ::Eldritch2::float16	KeyTime;

// ---------------------------------------------------

	struct CompressedUnitQuaternion {
		::Eldritch2::uint32	reconstructedComponentIndex : 2;
		::Eldritch2::uint32	component0 : 10;
		::Eldritch2::uint32	component1 : 10;
		::Eldritch2::uint32	component2 : 10;
	};

}	// namespace Animation
}	// namespace Eldritch2