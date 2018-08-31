/*==================================================================*\
  Allocator.cpp
  ------------------------------------------------------------------
  Purpose:
  Definition of a basic allocator object that can be used to manage
  memory. Note that this class uses the slightly different (and
  vastly superior) per-instance Lakos allocator model (see "Towards
  a Better Allocator Model" for details) as opposed to the standard
  per-type C++ model. If compatibility with the old style allocator
  is of concern, see the accompanying wrapper class defined in
  ETStdAllocator.hpp.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/Allocator.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

Allocator::Allocator(const Utf8Char* const name) ETNoexceptHint {
#if ET_DEBUG_BUILD
	_name = name;
#else
	ETUnreferencedParameter(name);
#endif
}

// ---------------------------------------------------

const Utf8Char* Allocator::GetName() const ETNoexceptHint {
#if ET_DEBUG_BUILD
	return _name;
#else
	return "<Name unavailable in release build>";
#endif
}

} // namespace Eldritch2
