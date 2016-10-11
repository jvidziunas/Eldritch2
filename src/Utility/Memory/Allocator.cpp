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
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Math/StandardLibrary.hpp>
#include <Utility/Memory/Allocator.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <EASTL/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	Allocator::Allocator( const Utf8Char* const name ) : _name( name ) {}

// ---------------------------------------------------

	const Utf8Char* Allocator::GetName() const {
		return _name;
	}

// ---------------------------------------------------

	void Allocator::SetName( const Eldritch2::Utf8Char* const name ) {
		_name = name;
	}

}	// namespace Eldritch2