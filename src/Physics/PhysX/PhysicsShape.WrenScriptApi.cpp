/*==================================================================*\
  PhysicsShape.WrenScriptApi.cpp
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
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsShape.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

double wrenGetSlotDouble(WrenVM* vm, int slot);

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS(PhysicsShape) {
		api.DefineClass<PhysicsShape>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "PhysicsShape", // clang-format off
			{ /* Static methods */
				ForeignMethod("ofCapsule(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<PhysicsShape>(vm, PxCapsuleGeometry(wrenGetSlotDouble(vm, 1), wrenGetSlotDouble(vm, 2)));
				}),
				ForeignMethod("ofSphere(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<PhysicsShape>(vm, PxSphereGeometry(wrenGetSlotDouble(vm, 1)));
				}),
				ForeignMethod("ofBox(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<PhysicsShape>(vm, PxBoxGeometry(AsPxVec3(wrenGetSlotAs<Vector>(vm, 1))));
				}) },
			{ /* Methods */ }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
