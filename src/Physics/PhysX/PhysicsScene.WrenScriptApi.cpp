/*==================================================================*\
  PhysicsScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
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

	namespace {

		template <typename Hit, PxU32 BufferSize>
		class WrenHitCallback : public PxHitCallback<Hit> {
			// - CONSTRUCTOR/DESTRUCTOR -------------------------

		public:
			//!	Constructs this @ref WrenHitCallback instance.
			ETInlineHint WrenHitCallback(WrenVM* vm, WrenHandle* arity1Call, WrenHandle* callback) ETNoexceptHint : PxHitCallback<Hit>(_buffer, BufferSize),
																													_vm(vm),
																													_arity1Call(arity1Call),
																													_callback(callback),
																													_wrenError(nullptr) {}
			//!	Constructs this @ref WrenHitCallback instance.
			ETInlineHint WrenHitCallback(WrenHitCallback&&) = default;

			~WrenHitCallback() override {
				if (_wrenError) {
					wrenSetSlotHandle(_vm, 0, _wrenError);
					wrenAbortFiber(_vm, 0);
				}

				wrenReleaseHandle(_vm, _callback);
			}

			// ---------------------------------------------------

		public:
			PxAgain processTouches(const Hit* buffer, PxU32 nbHits) override ETNoexceptHint {
				//	Set slot to contact info object.
				wrenSetSlotNull(_vm, 1);
				while (nbHits) {
					wrenSetSlotHandle(_vm, 0, _callback);
					if (wrenCall(_vm, _arity1Call) != WREN_RESULT_SUCCESS) {
						_wrenError = wrenGetSlotHandle(_vm, 0);
						return false;
					} else if (wrenGetSlotBool(_vm, 0) == false) {
						return false;
					}
				}

				return true;
			}

			// - DATA MEMBERS ------------------------------------

		private:
			WrenVM*     _vm;
			WrenHandle* _arity1Call;
			WrenHandle* _callback;
			WrenHandle* _wrenError;
			Hit         _buffer[BufferSize];
		};

	} // anonymous namespace

	ET_IMPLEMENT_WREN_CLASS(PhysicsScene) {
		api.DefineClass<PhysicsScene>(ET_BUILTIN_WREN_MODULE_NAME(Physics), "PhysicsSceneClass", // clang-format off
			{/*	Static methods */},
			{/*	Methods */
				ForeignMethod("eachAlongRay(_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					ForEachAlongRay(
						GetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						AsPxVec3(GetSlotAs<Vector>(vm, 1)),
						AsPxVec3(GetSlotAs<Vector>(vm, 2)),
						float32(wrenGetSlotDouble(vm, 3)),
						WrenHitCallback<PxRaycastHit, 8u>(vm, GetContext(vm)->GetCallStubForArity<1>(), wrenGetSlotHandle(vm, 4)));
				}),
				ForeignMethod("eachAlongRay(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					ForEachAlongRay(
						GetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						AsPxVec3(GetSlotAs<Transformation>(vm, 1).translation),
						AsPxVec3(GetSlotAs<Transformation>(vm, 1).rotation.GetForward()),
						float32(wrenGetSlotDouble(vm, 2)),
						WrenHitCallback<PxRaycastHit, 8u>(vm, GetContext(vm)->GetCallStubForArity<1>(), wrenGetSlotHandle(vm, 3)));
				}),
				ForeignMethod("eachWithinShape(_,_,_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					ForEachOverlap(
						GetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						GetSlotAs<PhysicsShape>(vm, 1).asGeometry,
						AsPxTransform(GetSlotAs<Transformation>(vm, 2)),
						AsPxVec3(GetSlotAs<Vector>(vm, 3)),
						float32(wrenGetSlotDouble(vm, 4)),
						WrenHitCallback<PxSweepHit, 8u>(vm, GetContext(vm)->GetCallStubForArity<1>(), wrenGetSlotHandle(vm, 5)));
				}),
				ForeignMethod("eachWithinShape(_,_,_)", [](WrenVM* vm) ETNoexceptHint {
					ForEachOverlap(
						GetSlotAs<PhysicsScene>(vm, 0).GetScene(),
						GetSlotAs<PhysicsShape>(vm, 1).asGeometry,
						AsPxTransform(GetSlotAs<Transformation>(vm, 2)),
						WrenHitCallback<PxOverlapHit, 8u>(vm, GetContext(vm)->GetCallStubForArity<1>(), wrenGetSlotHandle(vm, 3)));
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Physics::PhysX
