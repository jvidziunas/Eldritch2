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

//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Quaternion;
}

namespace Eldritch2 {
namespace Animation {

	using BoneIndex = int16;
	using KeyTime	= float16;

// ---

	struct CompressedUnitQuaternion {
		operator Quaternion() const;

	// - DATA MEMBERS ------------------------------------

		uint32	reconstructedComponentIndex : 2;
		uint32	component0					: 10;
		uint32	component1					: 10;
		uint32	component2					: 10;
	};

}	// namespace Animation
}	// namespace Eldritch2