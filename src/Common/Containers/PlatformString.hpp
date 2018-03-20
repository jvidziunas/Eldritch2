/*==================================================================*\
  PlatformString.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractString.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Allocator = MallocAllocator>
	using PlatformString = AbstractString<PlatformChar, Allocator>;

}	// namespace Eldritch2