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
#if( ET_COMPILER_IS_MSVC )
//	MSVC complains about macro redefinitions, since a few DirectX components separately
//	define some HRESULT values without an include guard. The definitions themselves are consistent,
//	so just disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 4005 )
#endif
#include <microprofile/microprofile.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
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