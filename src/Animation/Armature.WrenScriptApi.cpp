/*==================================================================*\
  Armature.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Animation {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;

	ET_IMPLEMENT_WREN_CLASS(Armature) {
		api.DefineClass<Armature>(ET_BUILTIN_WREN_MODULE_NAME(Animation), "Armature", // clang-format off
			{/*	Static methods */
				ForeignMethod("new(_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Armature>(vm, /*classSlot =*/0, GetSlotAs<Transformation>(vm, 1));
				}), },
			{/*	Methods */
				ForeignMethod("localToWorld", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, GetSlotAs<Armature>(vm, 0).GetLocalToWorld());
				}),
				ForeignMethod("localToWorld=(_)", [](WrenVM* vm) ETNoexceptHint {
					GetSlotAs<Armature>(vm, 0).SetLocalToWorld(GetSlotAs<Transformation>(vm, 1));
				}),
				ForeignMethod("worldToLocal", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, GetSlotAs<Armature>(vm, 0).GetWorldToLocal());
				}), }); // clang-format on
	}

}} // namespace Eldritch2::Animation
