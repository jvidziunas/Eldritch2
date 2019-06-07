/*==================================================================*\
  PhysicsScene.WrenScriptApi.cpp
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
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsShape.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::physx;

	// ---------------------------------------------------

	namespace {

		template <typename Hit>
		class WrenHitCallback : public PxHitCallback<Hit> {
			// - CONSTRUCTOR/DESTRUCTOR -------------------------

		public:
			//!	Constructs this @ref WrenHitCallback instance.
			template <PxU32 BufferSize>
			ETForceInlineHint WrenHitCallback(WrenVM* vm, WrenHandle* receiver, Hit (&buffer)[BufferSize]) ETNoexceptHint : PxHitCallback<Hit>(buffer, BufferSize),
																															_executor(GetExecutor(vm)),
																															_receiver(receiver),
																															_wrenError(nullptr) {}
			//!	Constructs this @ref WrenHitCallback instance.
			ETForceInlineHint WrenHitCallback(WrenHitCallback&&) ETNoexceptHint = default;

			~WrenHitCallback() override = default;

			// ---------------------------------------------------

		public:
			PxAgain processTouches(const Hit* buffer, PxU32 nbHits) ETNoexceptHint override {
				while (nbHits && _executor->Call(_receiver, nullptr)) {
				}

				return true;
			}

			// - DATA MEMBERS ------------------------------------

		private:
			ScriptExecutor* _executor;
			WrenHandle*     _receiver;
		};

		// ---------------------------------------------------

		ETForceInlineHint PxVec3 ETSimdCall wrenGetSlotPxVec3(WrenVM* vm, int slot) ETNoexceptHint {
			return AsPxVec3(wrenGetSlotAs<Vector>(vm, slot));
		}

		// ---------------------------------------------------

		ETForceInlineHint PxTransform ETSimdCall wrenGetSlotPxTransform(WrenVM* vm, int slot) ETNoexceptHint {
			return AsPxTransform(wrenGetSlotAs<Transformation>(vm, slot));
		}

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(PhysicsScene, api, moduleName) {
		api.DefineClass<PhysicsScene>("PhysicsSceneClass", moduleName, // clang-format off
			{/*	Static methods */},
			{/*	Methods */
				ForeignMethod("eachAlongRay(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					PxRaycastHit hits[8];
					ForEachAlongRay(
						wrenGetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						wrenGetSlotPxVec3(vm, 1),
						wrenGetSlotPxVec3(vm, 2),
						float32(wrenGetSlotDouble(vm, 3)),
						WrenHitCallback<PxRaycastHit>(vm, wrenGetSlotHandle(vm, 4), hits));
				}),
				ForeignMethod("eachAlongRay(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					const Transformation origin(wrenGetSlotAs<Transformation>(vm, 1));
					PxRaycastHit hits[8];
					ForEachAlongRay(
						wrenGetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						AsPxVec3(origin.translation),
						AsPxVec3(origin.rotation.GetForward()),
						float32(wrenGetSlotDouble(vm, 2)),
						WrenHitCallback<PxRaycastHit>(vm, wrenGetSlotHandle(vm, 3), hits));
				}),
				ForeignMethod("eachWithinShape(_,_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					PxSweepHit hits[8];
					ForEachOverlap(
						wrenGetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						wrenGetSlotAs<PhysicsShape>(vm, 1),
						wrenGetSlotPxTransform(vm, 2),
						wrenGetSlotPxVec3(vm, 3),
						float32(wrenGetSlotDouble(vm, 4)),
						WrenHitCallback<PxSweepHit>(vm, wrenGetSlotHandle(vm, 5), hits));
				}),
				ForeignMethod("eachWithinShape(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					PxOverlapHit hits[8];
					ForEachOverlap(
						wrenGetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						wrenGetSlotAs<PhysicsShape>(vm, 1),
						AsPxTransform(wrenGetSlotAs<Transformation>(vm, 2)),
						WrenHitCallback<PxOverlapHit>(vm, wrenGetSlotHandle(vm, 3), hits));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
