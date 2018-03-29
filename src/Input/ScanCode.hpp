/*==================================================================*\
  ScanCode.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Input {

	using ScanCode = uint8;

// ---

	struct Binding {
		uint8	actionId;
		int16	weight;
	};

// ---

	template <class Allocator = MallocAllocator>
	using BindingMap = ArrayMap<ScanCode, Binding, LessThan<ScanCode>, Allocator>;

}	// namespace Input
}	// namespace Eldritch2