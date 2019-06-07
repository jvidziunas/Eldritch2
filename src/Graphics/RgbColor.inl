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

	ETConstexpr ETForceInlineHint RgbColor::RgbColor(float32 red, float32 green, float32 blue) ETNoexceptHint : _channels { red, green, blue } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 RgbColor::GetRed() const ETNoexceptHint {
		return _channels[0];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 RgbColor::GetGreen() const ETNoexceptHint {
		return _channels[1];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 RgbColor::GetBlue() const ETNoexceptHint {
		return _channels[2];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint float32 RgbColor::GetRec709Luminance() const ETNoexceptHint {
		return (_channels[0] * 0.2126f) + (_channels[1] * 0.7152f) + (_channels[2] * 0.0722f);
	}

}} // namespace Eldritch2::Graphics
