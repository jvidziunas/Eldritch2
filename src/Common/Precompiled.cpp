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
ET_PUSH_COMPILER_WARNING_STATE()
/*	(4005) MSVC complains about macro redefinitions, since a few DirectX components separately define some HRESULT values without an include guard.
*	The definitions themselves are consistent, so just disable the warning.
*	(4201) Nameless struct/union
*	(6255) _alloca use
*	(4996) Insecure (not *_s) CRT function usage
*	(6387) Lack of NULL checking
*	(28251) Not decorating Win32 function prototypes with SAL markup
*	(6262) Stack allocation of big (32kb) buffer in printf implementation */
ET_SET_MSVC_WARNING_STATE(disable : 4005 4189 6330 4201 6255 4996 6387 28251 6262)
#include <microprofile/microprofile.cpp>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
#if ET_PLATFORM_WINDOWS
ET_LINK_LIBRARY("Ws2_32.lib")
#endif
//------------------------------------------------------------------//
