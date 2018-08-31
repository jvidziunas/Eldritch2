/*==================================================================*\
  XAudio2Listener.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/Xaudio2Listener.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

void wrenSetSlotHandle(WrenVM*, int, WrenHandle*);

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(XAudio2Listener) { // clang-format off
		api.CreateClass<XAudio2Listener>(ET_BUILTIN_WREN_MODULE_NAME(Audio), "AudioListener",
			{ /* Constructors */
				ConstructorMethod("new(_,_)", [](WrenVM* vm) {
					SetReturn<XAudio2Listener>(vm, GetSlotAs<Transformation>(vm, 2));
				})},
			{ /* Static methods */ },
			{ /* Properties */
				DefineGetter("localToWorld", [](WrenVM* vm) {
					const XAudio2Listener& self(GetSlotAs<XAudio2Listener>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Transformation>());
					SetReturn<Transformation>(vm, self.GetLocalToWorld());
				}),
				DefineGetter("worldToLocal", [](WrenVM* vm) {
					const XAudio2Listener& self(GetSlotAs<XAudio2Listener>(vm, 0));

					wrenSetSlotHandle(vm, 0, GetContext(vm)->FindForeignClass<Transformation>());
					SetReturn<Transformation>(vm, self.GetWorldToLocal());
				})},
			{ /* Methods */ });
	} // clang-format on

}}} // namespace Eldritch2::Audio::XAudio2
