/*==================================================================*\
  AnimationTypes.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {
	
	CompressedUnitQuaternion::operator Quaternion() const {
	/*	Vector	temp( static_cast<float32>(component0), static_cast<float32>(component1), static_cast<float32>(component2), 0.0f );
		temp *= (1.0f / 1024.0f);

		switch( reconstructedComponentIndex ) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		};*/

		return Quaternion();
	}

}	// namespace Animation
}	// namespace Eldritch2