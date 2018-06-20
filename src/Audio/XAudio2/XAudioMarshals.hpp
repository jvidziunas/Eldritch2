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
#include <X3DAudio.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Vector;
}

namespace Eldritch2 {
	namespace Audio {
		namespace XAudio2 {

			ETPureFunctionHint X3DAUDIO_VECTOR ETSimdCall	AsX3dAudioVector(Vector value);
			ETPureFunctionHint Vector ETSimdCall			AsVector(X3DAUDIO_VECTOR value);

		}	// namespace XAudio2
	}	// namespace Audio
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Audio/XAudio2/XAudioMarshals.inl>
//------------------------------------------------------------------//