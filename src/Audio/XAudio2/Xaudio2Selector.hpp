/*==================================================================*\
  Xaudio2Selector.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#if (_WIN32_WINNT < _WIN32_WINNT_WIN8)
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2FX.h>
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/X3DAudio.h>
#	include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#else
#	include <X3DAudio.h>
#	include <XAudio2.h>
#endif
//------------------------------------------------------------------//
