/*==================================================================*\
  CPPFormatDefinitions.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#define FMT_EXCEPTIONS 0
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <EABase/eabase.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
#include <cppformat/fmt/format.cc>
//------------------------------------------------------------------//