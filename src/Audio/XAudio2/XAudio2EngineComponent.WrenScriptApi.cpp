/*==================================================================*\
  XAudio2WorldComponent.WrenScriptApi.cpp
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
#include <Audio/XAudio2/XAudio2EngineComponent.hpp>
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {
	ET_DECLARE_WREN_CLASS(XAudio2AudioScene);
	ET_DECLARE_WREN_CLASS(XAudio2Listener);
}}} // namespace Eldritch2::Audio::XAudio2

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	void XAudio2EngineComponent::WorldComponent::PublishApi(ApiBuilder& api) {
		static ETConstexpr StringSpan AudioModule(ET_BUILTIN_WREN_MODULE_NAME(Audio));

		ET_REGISTER_WREN_CLASS(XAudio2AudioScene, api, AudioModule);
		ET_REGISTER_WREN_CLASS(XAudio2Listener, api, AudioModule);

		_scene = api.DefineVariable<XAudio2AudioScene>("AudioScene", AudioModule);
	}

}}} // namespace Eldritch2::Audio::XAudio2
