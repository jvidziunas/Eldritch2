/*==================================================================*\
  XAudio2Listener.WrenScriptApi.cpp
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
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/XAudio2Listener.hpp>
#include <Scripting/Wren/ScriptExecutor.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(XAudio2Listener, api, moduleName) {
		api.DefineClass<XAudio2Listener>("AudioListener", moduleName, // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<XAudio2Listener>(vm, /*classSlot=*/0, wrenGetSlotAs<Transformation>(vm, 2), /*linearVelocity =*/Vector::MakeIdentity());
				}), },
			{ /* Methods */
				ForeignMethod("localToWorld", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, wrenGetReceiver<XAudio2Listener>(vm).GetLocalToWorld());
				}),
				ForeignMethod("worldToLocal", [](WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Transformation>(vm, ~wrenGetReceiver<XAudio2Listener>(vm).GetLocalToWorld());
				}),
				ForeignMethod("linearVelocity", [] (WrenVM* vm) ETNoexceptHint {
					wrenSetReturn<Vector>(vm, wrenGetReceiver<XAudio2Listener>(vm).GetLinearVelocity());
				}),
				ForeignMethod("moveTo(_,_)", [] (WrenVM* vm) ETNoexceptHint {
					wrenGetReceiver<XAudio2Listener>(vm).MoveTo(wrenGetSlotAs<Transformation>(vm, 1), wrenGetSlotAs<Vector>(vm, 2));
				}),
				ForeignMethod("moveTo(_)", [] (WrenVM* vm) ETNoexceptHint {
					wrenGetReceiver<XAudio2Listener>(vm).MoveTo(wrenGetSlotAs<Transformation>(vm, 1), wrenGetReceiver<XAudio2Listener>(vm).GetLinearVelocity());
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Audio::XAudio2
