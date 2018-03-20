/*==================================================================*\
  TriggerArea.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsShape.hpp>
#include <Physics/PhysX/PhysicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
#include <Physics/PhysX/TriggerArea.hpp>
#include <Scripting/Wren/Context.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *	We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <PxPhysics.h>
ET_POP_COMPILER_WARNING_STATE()
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS( TriggerArea ) {
		api.CreateClass<TriggerArea>( ET_BUILTIN_WREN_MODULE_NAME( Physics ), "TriggerArea",
			{/* Constructors */
				DefineConstructor<TriggerArea ( PhysicsScene, Transformation, PhysicsShape )>( "new", [] ( WrenVM* vm ) {
					PhysxPointer<PxRigidStatic> actor( PxGetPhysics().createRigidStatic( AsPxTransform( GetSlotAs<Transformation>( vm, 2 ) ) ) );
					PhysxPointer<PxShape>		shape( PxGetPhysics().createShape(
						GetSlotAs<PhysicsShape>( vm, 3 ).asGeometry,
						GetSlotAs<PhysicsScene>( vm, 1 ).GetTriggerMaterial(),
						true,	// Shapes are owned by the trigger that created them.
						PxShapeFlags( PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE )
					) );

					ET_ABORT_WREN_UNLESS( actor, "Error creating PhysX actor for trigger!" );
					ET_ABORT_WREN_UNLESS( shape, "Error creating PhysX shape for trigger!" );

					SetReturn<TriggerArea>( vm, eastl::move( actor ), eastl::move( shape ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */
				DefineMethod<void ()>( "enable", [] ( WrenVM* vm ) {
					GetSlotAs<TriggerArea>( vm, 0 ).Enable();
				} ),
				DefineMethod<void ()>( "disable", [] ( WrenVM* vm ) {
					GetSlotAs<TriggerArea>( vm, 0 ).Disable();
				} )
			},
			{/*	Operators */}
		);
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2