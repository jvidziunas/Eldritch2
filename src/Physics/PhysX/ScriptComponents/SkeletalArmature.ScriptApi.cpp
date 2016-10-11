/*==================================================================*\
  SkeletalArmature.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/SkeletalArmature.hpp>
#include <Physics/PhysX/AssetViews/ArmatureView.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Assets/AssetLibrary.hpp>
//------------------------------------------------------------------//
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {
namespace {

	SkeletalArmature* CreateSkeletalArmature( PxScene& scene, const AssetLibrary& assetLibrary, const Utf8String<>& assetName ) {
		const auto	asset( assetLibrary.ResolveView<AssetViews::ArmatureView>( assetName ) );
		auto		articulation( asset->CreateArticulation( PxGetPhysics() ) );

		scene.addArticulation( *articulation );

		return nullptr;
	}

}	// anonymous namespace

	void SkeletalArmature::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineUserAllocatedClass<SkeletalArmature>(	{/* Factories */
																	DefineOverloadedFactory( SkeletalArmature* (*)( Injected<PxScene&, const AssetLibrary&>, const Utf8String<>& ), CreateSkeletalArmature )
																},
																{/*	Properties */},
																{/*	Methods */},
																{/*	Operators */},
																{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2