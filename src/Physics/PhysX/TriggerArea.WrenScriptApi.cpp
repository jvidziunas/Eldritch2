/*==================================================================*\
  TriggerArea.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsShape.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Physics/PhysX/TriggerArea.hpp>
#include <Scripting/Wren/Marshal.hpp>
//------------------------------------------------------------------//
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS(TriggerArea) {
		api.DefineClass<TriggerArea>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "TriggerArea", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					PhysicsScene&               scene(GetSlotAs<PhysicsScene>(vm, 1));
					PhysxPointer<PxRigidStatic> actor(PxGetPhysics().createRigidStatic(AsPxTransform(GetSlotAs<Transformation>(vm, 2))));
					ET_ABORT_WREN_UNLESS(actor, "Error creating PhysX actor for trigger.");

					PhysxPointer<PxShape> shape(PxGetPhysics().createShape(
						GetSlotAs<PhysicsShape>(vm, 3).asGeometry,
						scene.GetTriggerMaterial(),
						/*isExclusive =*/true, // Shapes are owned by the trigger that created them.
						PxShapeFlags(PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE)));
					ET_ABORT_WREN_UNLESS(shape, "Error creating PhysX shape for trigger.");

					SetReturn<TriggerArea>(vm, /*classSlot =*/0, eastl::move(actor), eastl::move(shape));
				}), },
			{ /* Methods */
				ForeignMethod("enable()", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<TriggerArea>(vm, 0).Enable();
				}),
				ForeignMethod("disable()", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<TriggerArea>(vm, 0).Disable();
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
