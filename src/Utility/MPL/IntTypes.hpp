/*==================================================================*\
  IntTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  As C++ does not currently have a standardized inttypes.h header
  for working with fixed-width types, we'll just roll our own.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <cstdint>
#include <cstddef>
//------------------------------------------------------------------//

namespace Eldritch2 {

	using int8		= ::std::int8_t;
	using uint8		= ::std::uint8_t;
	using int16		= ::std::int16_t;
	using uint16	= ::std::uint16_t;
	using int32		= ::std::int32_t;
	using uint32	= ::std::uint32_t;
	using int64		= ::std::int64_t;
	using uint64	= ::std::uint64_t;
	using intmax	= ::std::intmax_t;
	using uintmax	= ::std::uintmax_t;

	using ptrdiff	= ::std::ptrdiff_t;
	using intptr	= ::std::intptr_t;
	using uintptr	= ::std::uintptr_t;

}	// namespace Eldritch2