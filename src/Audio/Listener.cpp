/*==================================================================*\
  Listener.cpp
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
#include <Audio/Listener.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio {

	DiegeticVoice::DiegeticVoice(Transformation localToWorld, MicrosecondTime startTime, float32 loudnessDb) ETNoexceptHint : localToWorld(localToWorld), startTime(startTime), loudnessDb(loudnessDb) {}

}} // namespace Eldritch2::Audio
