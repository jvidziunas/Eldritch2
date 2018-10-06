/*==================================================================*\
  XAudio2AudioScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/XAudio2/XAudio2AudioScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS(XAudio2AudioScene) {
		api.DefineClass<XAudio2AudioScene>(ET_BUILTIN_WREN_MODULE_NAME(Audio), "AudioSceneClass", { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Audio::XAudio2
