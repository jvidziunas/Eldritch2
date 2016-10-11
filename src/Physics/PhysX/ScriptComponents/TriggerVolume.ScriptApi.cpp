/*==================================================================*\
  TriggerVolume.ScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/ScriptComponents/TriggerVolume.hpp>
#include <Physics/PhysX/TypeMarshalers.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Scripting/ApiRegistrar.hpp>
//------------------------------------------------------------------//
#include <geometry/PxSimpleTriangleMesh.h>
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace ScriptComponents {
namespace {

	void ConstructTriggerVolume( void* thisPointer, PxScene& scene, const RigidTransform& transform, const Float4& halfExtent ) {
		UniquePointer<PxRigidStatic>	actor( PxGetPhysics().createRigidStatic( PxTransformFromRigidTransform( transform ) ) );
		UniquePointer<PxShape>			shape( PxGetPhysics().createShape( PxBoxGeometry( PxVec3FromFloat4( halfExtent ) ), nullptr, 0, true, PxShapeFlag::eTRIGGER_SHAPE ) );

		actor->attachShape( *shape );

		scene.addActor( *actor );

		new(thisPointer) TriggerVolume( eastl::move( actor ), eastl::move( shape ) );
	}

}	// anonymous namespace

	void TriggerVolume::RegisterScriptApi( ApiRegistrar& registrar ) {
		registrar.DefineClass<TriggerVolume>(	{/* Constructors */
													DefineOverloadedConstructor( void (*)( void*, Injected<PxScene&>, const RigidTransform&, const Float4& ), ConstructTriggerVolume )
												},
												{/*	Properties */},
												{/*	Methods */
												//	DefineMethod( "Enable", TriggerVolume::Enable ),
												//	DefineMethod( "Disable", TriggerVolume::Disable )
												},
												{/*	Operators */},
												{/*	Fields */} );
	}

}	// namespace ScriptComponents
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2