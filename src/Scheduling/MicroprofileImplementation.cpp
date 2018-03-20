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
#include <EABase/eabase.h>
#ifdef UNICODE
#	undef UNICODE
#endif
//	Don't use the thread name callback, we provide the name ourselves in the scheduler classes and don't need the 'retained mode' callback.
#define MICROPROFILE_USE_THREAD_NAME_CALLBACK 0
#define MICROPROFILE_GPU_TIMERS 0
ET_PUSH_COMPILER_WARNING_STATE()
/*	(4005) MSVC complains about macro redefinitions, since a few DirectX components separately define some HRESULT values without an include guard.
 *	The definitions themselves are consistent, so just disable the warning.
 *	(4201) Nameless struct/union
 *	(6255) _alloca use
 *	(4996) Insecure (not *_s) CRT function usage
 *	(6387) Lack of NULL checking
 *	(28251) Not decorating Win32 function prototypes with SAL markup
 *	(6262) Stack allocation of big (32kb) buffer in printf implementation */
ET_SET_MSVC_WARNING_STATE( disable : 4005 4189 6330 4201 6255 4996 6387 28251 6262 )
#include <microprofile/microprofile.cpp>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
	ET_LINK_LIBRARY( "Ws2_32.lib" )
#endif
//------------------------------------------------------------------//