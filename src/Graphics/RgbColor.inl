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

	ETInlineHint float16 RgbColor::GetRed() const {
		return _channels[0];
	}

	// ---------------------------------------------------

	ETInlineHint float16 RgbColor::GetGreen() const {
		return _channels[1];
	}

	// ---------------------------------------------------

	ETInlineHint float16 RgbColor::GetBlue() const {
		return _channels[2];
	}

	// ---------------------------------------------------

	ETInlineHint float32 RgbColor::GetRec709Luminance() const {
		return (_channels[0] * 0.2126f) + (_channels[1] * 0.7152f) + (_channels[2] * 0.0722f);
	}

}} // namespace Eldritch2::Graphics
