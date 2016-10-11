/*==================================================================*\
  FloatTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  Rough equivalent to the IntTypes.hpp header, modified to instead
  provide a baseline for floating-point types instead of fixed.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <EABase/eabase.h>
#if( ET_COMPILER_IS_MSVC )
// (4804) Disable warning about booleans being used as an argument to the shift operator.
// (6326) Disable warning about comparing constants with other constants.
#	pragma warning( push )
#	pragma warning( disable : 4804 )
#	pragma warning( disable : 6326 )
#endif
#include <half/include/half.hpp>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

namespace Eldritch2 {

	using float16	= half_float::half;
	using float32	= float;
	using float64	= double;

}	// namespace Eldritch2