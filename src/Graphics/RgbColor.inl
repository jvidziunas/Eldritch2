/*==================================================================*\
  RgbColor.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETInlineHint ETForceInlineHint RgbColor::RgbColor(float32 red, float32 green, float32 blue) :
		_channels { float16(red), float16(green), float16(blue) } {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float16 RgbColor::GetRed() const ETNoexceptHint {
		return _channels[0];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float16 RgbColor::GetGreen() const ETNoexceptHint {
		return _channels[1];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float16 RgbColor::GetBlue() const ETNoexceptHint {
		return _channels[2];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint float32 RgbColor::GetRec709Luminance() const ETNoexceptHint {
		return (_channels[0] * 0.2126f) + (_channels[1] * 0.7152f) + (_channels[2] * 0.0722f);
	}

}} // namespace Eldritch2::Graphics
