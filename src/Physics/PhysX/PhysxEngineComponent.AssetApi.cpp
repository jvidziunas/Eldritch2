/*==================================================================*\
  PhysxEngineComponent.AssetApi.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureAsset.hpp>
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Physics/PhysX/PhysxEngineComponent.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Assets;

	void PhysxEngineComponent::PublishAssetTypes(AssetApiBuilder& api) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Asset factory registration", 0xBBBBBB);
		api.DefineType(ArmatureAsset::GetExtension(), AssetApiBuilder::DefaultFactory<ArmatureAsset>());
		api.DefineType(TerrainAsset::GetExtension(), AssetApiBuilder::DefaultFactory<TerrainAsset>());
		api.DefineType(PhysicsAsset::GetExtension(), AssetApiBuilder::DefaultFactory<PhysicsAsset>());
	}

}}} // namespace Eldritch2::Physics::PhysX
