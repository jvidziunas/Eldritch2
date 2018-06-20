/*==================================================================*\
  WrenEngineComponent.AssetApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/AssetViews/DialogueSetAsset.hpp>
#include <Scripting/Wren/AssetViews/ScriptAsset.hpp>
#include <Scripting/Wren/WrenEngineComponent.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {

			using namespace ::Eldritch2::Scripting::Wren::AssetViews;
			using namespace ::Eldritch2::Assets;

			void WrenEngineComponent::AcceptVisitor(AssetApiBuilder& factories) {
				MICROPROFILE_SCOPEI("Engine/Initialization", "Asset factory registration", 0xBBBBBB);

				factories.DefineType(DialogueSetAsset::GetExtension(), AssetApiBuilder::DefaultFactory<DialogueSetAsset>());
				factories.DefineType(ScriptAsset::GetExtension(), AssetApiBuilder::DefaultFactory<ScriptAsset>());
			}

		}	// namespace Wren
	}	// namespace Scripting
}	// namespace Eldritch2