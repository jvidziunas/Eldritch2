/*==================================================================*\
  Fmtlib.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#if !defined( FMT_EXCEPTIONS )
#	define FMT_EXCEPTIONS 0
#endif
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <EABase/eabase.h>
#include <fmt/format.cc>
//------------------------------------------------------------------//