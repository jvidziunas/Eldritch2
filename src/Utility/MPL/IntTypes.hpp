/*==================================================================*\
  IntTypes.hpp
  ------------------------------------------------------------------
  Purpose:
  As C++ does not currently have a standardized inttypes.h header
  for working with fixed-width types, we'll just roll our own.

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
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

	typedef ::std::int8_t		int8;
	typedef ::std::uint8_t		uint8;
	typedef ::std::int16_t		int16;
	typedef ::std::uint16_t		uint16;
	typedef ::std::int32_t		int32;
	typedef ::std::uint32_t		uint32;
	typedef ::std::int64_t		int64;
	typedef ::std::uint64_t		uint64;
	typedef ::std::intmax_t		intmax;
	typedef ::std::uintmax_t	uintmax;

	typedef ::std::ptrdiff_t	ptrdiff;
	typedef ::std::intptr_t		intptr;
	typedef ::std::uintptr_t	uintptr;

}	// namespace Eldritch2