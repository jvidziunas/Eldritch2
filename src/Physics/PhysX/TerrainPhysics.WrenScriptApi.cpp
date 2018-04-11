/*==================================================================*\
  TerrainPhysics.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
#include <Scripting/Wren/AssetReference.hpp>
#include <Physics/PhysX/TerrainPhysics.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <PxRigidStatic.h>
#	include <PxPhysics.h>
#	include <PxScene.h>
ET_POP_COMPILER_WARNING_STATE()
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
	
	using namespace ::Eldritch2::Physics::PhysX::AssetViews;
	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS( TerrainPhysics ) {
		api.CreateClass<TerrainPhysics>( ET_BUILTIN_WREN_MODULE_NAME( Physics ), "TerrainPhysics",
			{/* Constructors */
				DefineConstructor<TerrainPhysics ( PhysicsScene, AssetReference<TerrainAsset> )>( "new", [] ( WrenVM* vm ) {
					PhysxPointer<PxRigidStatic> actor( PxGetPhysics().createRigidStatic( PxTransform( PxIdentity ) ) );
					ET_ABORT_WREN_UNLESS( actor, "Error creating PhysX actor!" );

					actor->attachShape( GetSlotAs<AssetReference<TerrainAsset>>( vm, 2 ).Get().GetShape() );

					SetReturn<TerrainPhysics>( vm, eastl::move( actor ), GetSlotAs<AssetReference<TerrainAsset>>( vm, 2 ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ()>( "enable", [] ( WrenVM* vm ) {
					GetSlotAs<TerrainPhysics>( vm, 0 ).Enable();
				} ),
				DefineMethod<void ()>( "disable", [] ( WrenVM* vm ) {
					GetSlotAs<TerrainPhysics>( vm, 0 ).Disable();
				} )
			},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2