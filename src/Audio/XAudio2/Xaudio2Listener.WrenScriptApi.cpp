/*==================================================================*\
  XAudio2Listener.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Audio/XAudio2/Xaudio2Listener.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(XAudio2Listener) {
		api.DefineClass<XAudio2Listener>(ET_BUILTIN_WREN_MODULE_NAME(Audio), "AudioListener", // clang-format off
			{ /* Static methods */
				ForeignMethod("new(_,_)", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<XAudio2Listener>(vm, /*classSlot=*/0, GetSlotAs<Transformation>(vm, 2));
				}), },
			{ /* Methods */
				ForeignMethod("localToWorld", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, GetSlotAs<XAudio2Listener>(vm, 0).GetLocalToWorld());
				}),
				ForeignMethod("worldToLocal", [](WrenVM* vm) ETNoexceptHint {
					SetReturn<Transformation>(vm, GetSlotAs<XAudio2Listener>(vm, 0).GetWorldToLocal());
				}), }); // clang-format on
	}

}}} // namespace Eldritch2::Audio::XAudio2
