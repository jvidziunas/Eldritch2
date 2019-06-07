/*==================================================================*\
  XAudio2AudioScene.WrenScriptApi.cpp
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
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Audio { namespace XAudio2 {

	using namespace ::Eldritch2::Scripting::Wren;

	// ---------------------------------------------------

	ET_IMPLEMENT_WREN_CLASS(XAudio2AudioScene, api, moduleName) {
		api.DefineClass<XAudio2AudioScene>("AudioSceneClass", moduleName, { /* Static methods */ }, { /* Methods */ });
	}

}}} // namespace Eldritch2::Audio::XAudio2
