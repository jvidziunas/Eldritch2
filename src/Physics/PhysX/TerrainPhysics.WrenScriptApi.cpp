/*==================================================================*\
  TerrainPhysics.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
#include <Physics/PhysX/TerrainPhysics.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Core/ContentLocator.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxRigidStatic.h>
#include <PxPhysics.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(TerrainPhysics) {
		api.DefineClass<TerrainPhysics>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "TerrainPhysics", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					const TerrainAsset* const asset(Cast<TerrainAsset>(wrenGetSlotAs<AssetReference>(vm, 2)));
					ET_ABORT_WREN_IF(asset == nullptr, vm, "Asset must be a TerrainAsset.")

					PhysxPointer<PxRigidStatic> actor(PxGetPhysics().createRigidStatic(PxTransform(PxIdentity)));
					ET_ABORT_WREN_IF(actor == nullptr, vm, "Error creating PhysX actor.");

					actor->attachShape(asset->GetShape());
					wrenSetReturn<TerrainPhysics>(vm, Move(actor));
				}) },
			{ /* Methods */}); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
