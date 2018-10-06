/*==================================================================*\
  XaudioMarshals.inl
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

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	ETInlineHint ETPureFunctionHint X3DAUDIO_VECTOR ETSimdCall AsX3dAudioVector(Vector value) {
		ET16ByteAligned float32 coefficients[4];

		StreamCoefficients(coefficients, value);
		return { coefficients[Vector::X], coefficients[Vector::Y], coefficients[Vector::Z] };
	}

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint Vector ETSimdCall AsVector(X3DAUDIO_VECTOR value) {
		return { value.x, value.y, value.z, 1.0f };
	}

}}} // namespace Eldritch2::Audio::XAudio2
