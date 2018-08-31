/*==================================================================*\
  Profiler.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

#define ET_PROFILE_SCOPE(group, name, debugHexColor) MICROPROFILE_SCOPEI(group, name, debugHexColor)
#define ET_PROFILE_APP_BEGIN(name) MicroProfileInit()
#define ET_PROFILE_APP_END() MicroProfileShutdown()
#define ET_PROFILE_THREAD_BEGIN(name) MicroProfileOnThreadCreate(name)
#define ET_PROFILE_THREAD_END() MicroProfileOnThreadExit()
#define ET_PROFILE_FRAME_BEGIN_CPU()
#define ET_PROFILE_FRAME_BEGIN_GPU(context) MicroProfileFlip(context)

namespace Eldritch2 {

} // namespace Eldritch2
