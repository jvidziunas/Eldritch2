/*==================================================================*\
  RgbColor.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	RgbColor::RgbColor( float16 red, float16 green, float16 blue ) {
		_channels[0] = red;
		_channels[1] = green;
		_channels[2] = blue;
	}

// ---------------------------------------------------
	
	ETPureFunctionHint RgbColor	GetRgbFromCieXyz( float16 x, float16 y, float16 z ) {
		return RgbColor( x, y, z );
	}

// ---------------------------------------------------

	ETPureFunctionHint RgbColor	GetRgbFromKelvin( float32 degrees ) {
		return RgbColor( float16( degrees ), float16( degrees ), float16( degrees ) );
	}

}	// namespace Graphics
}	// namespace Eldritch2