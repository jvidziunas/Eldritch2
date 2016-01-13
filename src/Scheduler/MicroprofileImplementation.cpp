/*==================================================================*\
  MicroprofileImplementation.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/Thread.hpp>
#ifdef UNICODE
#	undef UNICODE
#endif
#define MICROPROFILE_USE_THREAD_NAME_CALLBACK 1
#define MICROPROFILE_GPU_TIMERS_D3D11 1
#define MICROPROFILE_IMPL
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
ET_LINK_LIBRARY( "Ws2_32.lib" )
#endif
//------------------------------------------------------------------//


#if defined( MICROPROFILE_USE_THREAD_NAME_CALLBACK ) && (0 != MICROPROFILE_USE_THREAD_NAME_CALLBACK)
MICROPROFILE_API const char* MicroProfileGetThreadName() {
	return "<Unknown>";
}
#endif