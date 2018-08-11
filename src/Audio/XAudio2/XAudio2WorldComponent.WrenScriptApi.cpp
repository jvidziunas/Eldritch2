/*==================================================================*\
  XAudio2WorldComponent.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2WorldComponent.hpp>
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	ET_DECLARE_WREN_CLASS(XAudio2AudioScene);
	ET_DECLARE_WREN_CLASS(XAudio2Listener);
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace Eldritch2::Scripting::Wren;

	void XAudio2WorldComponent::DefineScriptApi(ApiBuilder& api) {
		ET_REGISTER_WREN_CLASS(XAudio2AudioScene, api);
		ET_REGISTER_WREN_CLASS(XAudio2Listener, api);

		_scene = api.CreateVariable<XAudio2AudioScene>(ET_BUILTIN_WREN_MODULE_NAME(Audio), "AudioScene");
	}

}}} // namespace Eldritch2::Audio::XAudio2
