/*==================================================================*\
  Armature.WrenScriptApi.cpp
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
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Animation/AnimationApi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(Armature, api, moduleName) {
		api.DefineClass<Armature>("Armature", moduleName, // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Armature>(vm, /*classSlot =*/0, wrenGetSlotAs<Transformation>(vm, 1));
				}), },
			{/*	Methods */
				ForeignMethod("localToWorld", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, wrenGetSlotAs<Armature>(vm, 0).GetLocalToWorld());
				}),
				ForeignMethod("localToWorld=(_)", [](WrenVM* vm) ETNoexceptHint {
					wrenGetSlotAs<Armature>(vm, 0).SetLocalToWorld(wrenGetSlotAs<Transformation>(vm, 1));
				})}); // clang-format on
	}

}} // namespace Eldritch2::Animation
