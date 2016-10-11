/*==================================================================*\
  MeshCollider.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/SkeletalColliderView.hpp>
#include <Physics/PhysX/ScriptComponents/MeshCollider.hpp>
#include <Scripting/ApiRegistrar.hpp>
#include <Assets/AssetLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Assets;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {
namespace {

	void ConstructMeshCollider( void* thisPointer, const AssetLibrary& assetLibrary, ScriptComponents::SkeletalArmature& armature, const Utf8String<>& assetName ) {
		const auto	asset( assetLibrary.ResolveView<AssetViews::SkeletalColliderView>( assetName ) );
		
		new(thisPointer) MeshCollider( armature, *asset );
	}

}	// anonymous namespace

	void MeshCollider::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<MeshCollider>(	{/* Constructors */
													DefineOverloadedConstructor( void (*)( void*, Injected<const AssetLibrary&>, ScriptComponents::SkeletalArmature&, const Utf8String<>& ), ConstructMeshCollider )
												},
												{/*	Properties */},
												{/*	Methods */
												//	DefineMethod( "Enable", MeshCollider::Enable ),
												//	DefineMethod( "Disable", MeshCollider::Disable )
												},
												{/*	Operators */},
												{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2