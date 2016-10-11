/*==================================================================*\
  TerrainCollider.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/TerrainCollider.hpp>
#include <Physics/PhysX/AssetViews/TerrainView.hpp>
#include <Physics/PhysX/TypeMarshalers.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Physics::PhysX::AssetViews;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {
namespace {

	void ConstructTerrainCollider( void* thisPointer, PxScene& scene, const AssetLibrary& assetLibrary, const Utf8String<>& assetName ) {
		UniquePointer<PxRigidStatic>	actor( PxGetPhysics().createRigidStatic( PxTransform( PxIdentity ) ) );
		const auto						asset( assetLibrary.ResolveView<AssetViews::TerrainView>( assetName ) );

		scene.addActor( *actor );
		
		new(thisPointer) TerrainCollider( eastl::move( actor ), *asset );
	}

}	// anonymous namespace

	void TerrainCollider::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<TerrainCollider>( {/* Constructors */
													DefineOverloadedConstructor( void (*)( void*, Injected<PxScene&, const AssetLibrary&>, const Utf8String<>& ), ConstructTerrainCollider )
												},
												{/*	Properties */},
												{/*	Methods */
												//	DefineMethod( "Enable", TerrainCollider::Enable )
												//	DefineMethod( "Disable", TerrainCollider::Disable )
												},
												{/*	Operators */},
												{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2