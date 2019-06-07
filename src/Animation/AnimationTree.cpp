/*==================================================================*\
  AnimationTree.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/AnimationTree.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	Result AnimationTree::BindResources(const ArmatureDefinition& definition) {
		_definition = ETAddressOf(definition);

		return Result::Success;
	}

	// ---------------------------------------------------

	void AnimationTree::FreeResources() ETNoexceptHint {
		_clips.Clear();
		_definition = nullptr;
	}

}} // namespace Eldritch2::Animation
