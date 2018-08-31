/*==================================================================*\
  Precompiled.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//
#include <eastl/source/bonus/intrusive_slist.cpp>
#include <eastl/source/intrusive_list.cpp>
#include <eastl/source/numeric_limits.cpp>
#include <eastl/source/red_black_tree.cpp>
#include <eastl/source/hashtable.cpp>
#include <eastl/source/string.cpp>
#include <eastl/source/assert.cpp>
//------------------------------------------------------------------//
/*	(4005) MSVC complains about macro redefinitions, since a few DirectX components separately define some HRESULT values without an include guard.
 *	The definitions themselves are consistent, so just disable the warning.
 *	(4018) Signed/unsigned mismatch
 *	(4189) 
 *	(4201) Nameless struct/union
 *	(4456/6246) Declaration in tighter scope shadows variable in same/outer scope
 *	(4996) Insecure (not *_s) CRT function usage
 *	(6054) Filename may not be terminated
 *	(6255) _alloca use
 *	(6262) Stack allocation of big (32kb) buffer in printf implementation
 *	(6297) 32/64-bit shift interleave
 *	(6340) printf formatter mismatches
 *	(6385) Possible read overrun
 *	(6386) Possible write overrun
 *	(6387) Lack of NULL checking
 *	(28251) Not decorating Win32 function prototypes with SAL markup */
ET_PUSH_MSVC_WARNING_STATE(disable : 4005 4018 4189 4201 4456 4996 6054 6246 6255 6262 6297 6330 6340 6385 6386 6387 28251)
#include <microprofile/microprofile.cpp>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
ET_LINK_LIBRARY("Ws2_32.lib")
#endif
//------------------------------------------------------------------//
