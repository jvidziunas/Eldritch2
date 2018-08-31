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
#if ET_PLATFORM_WINDOWS
#	include <windows.h>
#endif
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/X3DAudio.h>
#else
#	include <X3DAudio.h>
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {
class Vector;
}

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	ETPureFunctionHint X3DAUDIO_VECTOR ETSimdCall AsX3dAudioVector(Vector value);
	ETPureFunctionHint Vector ETSimdCall AsVector(X3DAUDIO_VECTOR value);

}}} // namespace Eldritch2::Audio::XAudio2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/XAudio2/XAudioMarshals.inl>
//------------------------------------------------------------------//
