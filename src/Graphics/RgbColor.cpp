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
		namespace {

			ETInlineHint ETPureFunctionHint float32 ExpandChannel(uint8 channel) {
				constexpr float32 scale = 1.0 / 255.0;

				return static_cast<float32>(channel) * scale;
			}

		}	// anonymous namespace

		RgbColor::RgbColor(float32 red, float32 green, float32 blue) : _channels{ float16(red), float16(green), float16(blue) } {}

	// ---------------------------------------------------

		ETPureFunctionHint RgbColor	GetRgbFromCieXyz(float32 x, float32 y, float32 z) {
			return RgbColor(x, y, z);
		}

	// ---------------------------------------------------

		ETPureFunctionHint RgbColor	GetRgbFromEv(uint8 red, uint8 green, uint8 blue, float16 exposureStops) {
			const float32	scale(Pow(2.0f, float32(exposureStops) - 10.0f));

			return RgbColor(scale * ExpandChannel(red), scale * ExpandChannel(green), scale * ExpandChannel(blue));
		}

	// ---------------------------------------------------

		ETPureFunctionHint RgbColor	GetRgbFromKelvin(float32 degrees) {
			return RgbColor(degrees, degrees, degrees);
		}

	}	// namespace Graphics
}	// namespace Eldritch2