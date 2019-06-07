/*==================================================================*\
  PhysxEngineComponent.WrenScriptApi.cpp
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
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {
	ET_DECLARE_WREN_CLASS(CharacterPhysics);
	ET_DECLARE_WREN_CLASS(TerrainPhysics);
	ET_DECLARE_WREN_CLASS(PhysicsShape);
	ET_DECLARE_WREN_CLASS(PhysicsScene);
	ET_DECLARE_WREN_CLASS(TriggerArea);
	ET_DECLARE_WREN_CLASS(Physics);
}}} // namespace Eldritch2::Physics::PhysX

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::physx;

	// ---------------------------------------------------

	void PhysxEngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(CharacterPhysics, api);
		ET_REGISTER_WREN_CLASS(TerrainPhysics, api);
		ET_REGISTER_WREN_CLASS(PhysicsShape, api);
		ET_REGISTER_WREN_CLASS(PhysicsScene, api);
		ET_REGISTER_WREN_CLASS(TriggerArea, api);
		ET_REGISTER_WREN_CLASS(Physics, api);

		_scene = api.DefineVariable<PhysicsScene>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "PhysicsScene");
	}

}}} // namespace Eldritch2::Physics::PhysX
