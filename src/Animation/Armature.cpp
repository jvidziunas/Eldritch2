/*==================================================================*\
  Armature.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Animation {

		Armature::Armature(
			Transformation localToWorld
		) : _localToWorld(localToWorld),
			_attachPointsByTarget(MallocAllocator("Armature Attachment Point Collection Allocator")) {
		}

	}	// namespace Animation
}	// namespace Eldritch2