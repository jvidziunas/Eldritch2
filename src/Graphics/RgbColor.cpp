/*==================================================================*\
  RgbColor.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETPureFunctionHint RgbColor GetRgbFromCieXyz(float32 x, float32 y, float32 z) ETNoexceptHint {
		return RgbColor(x, y, z);
	}

	// ---------------------------------------------------

	ETPureFunctionHint RgbColor GetRgbFromEv(uint8 red, uint8 green, uint8 blue, float32 exposureStops) ETNoexceptHint {
		const float32 scale(1.0f / 255.0f);
		const float32 exposure(Pow(2.0f, exposureStops - 10.0f));

		return RgbColor(red * scale * exposure, green * scale * exposure, blue * scale * exposure);
	}

	// ---------------------------------------------------

	ETPureFunctionHint RgbColor GetRgbFromKelvin(float32 degrees) ETNoexceptHint {
		return RgbColor(degrees, degrees, degrees);
	}

}} // namespace Eldritch2::Graphics
