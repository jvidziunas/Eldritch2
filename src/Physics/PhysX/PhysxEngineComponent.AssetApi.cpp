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
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Assets;

	void PhysxEngineComponent::AcceptVisitor( AssetApiBuilder& api ) {
		MICROPROFILE_SCOPEI( "Engine/Initialization", "Asset factory registration", 0xBBBBBB );

		api.DefineType<ArmatureAsset>( ".e2armature" );
		api.DefineType<TerrainAsset>( ".e2terrain" );
		api.DefineType<PhysicsAsset>( ".e2physics" );
	}

}	// namespace PhysX
}	// namespace Eldritch2
}	// namespace Physics