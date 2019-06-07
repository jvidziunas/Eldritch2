/*==================================================================*\
  Physics.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/Physics.hpp>
//------------------------------------------------------------------//

//==================================================================//
// LIBRARIES
//==================================================================//
ET_LINK_DEBUGGABLE_LIBRARY("PhysX3CharacterKinematicDEBUG_x64.lib", "PhysX3CharacterKinematic_x64.lib");
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::physx;

	// ---------------------------------------------------

	Physics::Physics(const PhysicsAsset& asset, PhysxPointer<PxController> controller, PhysicsClip& clip) ETNoexceptHint : _asset(ETAddressOf(asset)), _controller(Move(controller)), _clip(ETAddressOf(clip)) {
		if (_controller) {
			_controller->setUserData(this);
		}
	}

	// ---------------------------------------------------

	void Physics::SetMovement(Vector displacement) ETNoexceptHint {
		_moveDisplacement = displacement;
	}

}}} // namespace Eldritch2::Physics::PhysX
