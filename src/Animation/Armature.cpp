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

namespace Eldritch2 { namespace Animation {

	Armature::Armature(
		Transformation localToWorld) :
		AnimationTree<>(MallocAllocator("Armature Animation Tree Root Allocator")),
		_localToWorld(localToWorld),
		_attachPointsByTarget(MallocAllocator("Armature Attachment Point Collection Allocator")) {
	}

}} // namespace Eldritch2::Animation
