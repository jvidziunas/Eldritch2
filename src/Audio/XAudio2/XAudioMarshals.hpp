/*==================================================================*\
  XaudioMarshals.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/Xaudio2Selector.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	ETPureFunctionHint X3DAUDIO_VECTOR ETSimdCall AsX3dAudioVector(Vector) ETNoexceptHint;
	ETPureFunctionHint Vector ETSimdCall AsVector(X3DAUDIO_VECTOR) ETNoexceptHint;

}}} // namespace Eldritch2::Audio::XAudio2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/XAudio2/XAudioMarshals.inl>
//------------------------------------------------------------------//
