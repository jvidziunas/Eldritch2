/*==================================================================*\
  Utf8String.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/String.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <class Allocator = Eldritch2::ChildAllocator>
	using Utf8String = Eldritch2::String<Eldritch2::Utf8Char, Allocator>;

}	// namespace Eldritch2