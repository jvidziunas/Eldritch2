/*==================================================================*\
  PhysxEngineComponent.AssetApi.cpp
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
#include <Physics/PhysX/AssetViews/ArmatureDefinitionAsset.hpp>
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Core/ContentDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Core;

	// ---------------------------------------------------

	void PhysxEngineComponent::PublishApi(AssetApiBuilder& api) {
		api.DefineType(ArmatureDefinitionAsset::GetExtension(), api.MakeFactory<ArmatureDefinitionAsset>());
		api.DefineType(TerrainAsset::GetExtension(), api.MakeFactory<TerrainAsset>());
		api.DefineType(PhysicsAsset::GetExtension(), api.MakeFactory<PhysicsAsset>());
	}

}}} // namespace Eldritch2::Physics::PhysX
