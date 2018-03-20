/*==================================================================*\
  PhysicsShape.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxMarshals.hpp>
#include <Physics/PhysX/PhysicsShape.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//
#include <wren.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	using namespace ::Eldritch2::Scripting::Wren;
	using namespace ::Eldritch2::Scripting;
	using namespace ::physx;

	ET_IMPLEMENT_WREN_CLASS( PhysicsShape ) {
		api.CreateClass<PhysicsShape>( ET_BUILTIN_WREN_MODULE_NAME( Physics ), "PhysicsShape",
			{/* Constructors */
				DefineConstructor<PhysicsShape ( double, double )>( "newCapsule", [] ( WrenVM* vm ) {
					SetReturn<PhysicsShape>( vm, PxCapsuleGeometry( wrenGetSlotDouble( vm, 1 ), wrenGetSlotDouble( vm, 2 ) ) );
				} ),
				DefineConstructor<PhysicsShape ( double )>( "newSphere", [] ( WrenVM* vm ) {
					SetReturn<PhysicsShape>( vm, PxSphereGeometry( wrenGetSlotDouble( vm, 1 ) ) );
				} ),
				DefineConstructor<PhysicsShape ( Vector )>( "newBox", [] ( WrenVM* vm ) {
					SetReturn<PhysicsShape>( vm, PxBoxGeometry( AsPxVec3( GetSlotAs<Vector>( vm, 1 ) ) ) );
				} )
			},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Operators */}
		);
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2