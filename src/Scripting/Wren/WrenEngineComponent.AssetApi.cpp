/*==================================================================*\
  WrenEngineComponent.AssetApi.cpp
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
#include <Scripting/Wren/AssetViews/SerializedWorldAsset.hpp>
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Core/ContentDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren {

	using namespace ::Eldritch2::Scripting::Wren::AssetViews;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	void WrenEngineComponent::PublishApi(AssetApiBuilder& api) {
		api.DefineType(SerializedWorldAsset::GetExtension(), api.MakeFactory<SerializedWorldAsset>());
		api.DefineType(DialogueSetAsset::GetExtension(), api.MakeFactory<DialogueSetAsset>());
		api.DefineType(ScriptAsset::GetExtension(), api.MakeFactory<ScriptAsset>());
	}

}}} // namespace Eldritch2::Scripting::Wren
