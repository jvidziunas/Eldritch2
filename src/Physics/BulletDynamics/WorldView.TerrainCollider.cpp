/*==================================================================*\
  WorldView.TerrainCollider.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	const char* const WorldView::TerrainCollider::scriptTypeName = UTF8L("TerrainCollider");

// ---------------------------------------------------

	WorldView::TerrainCollider::TerrainCollider( WorldView& owningView ) : _collisionShape( 0, 0, nullptr, 0.0f, 0.0f, 0.0f, 0, PHY_FLOAT, false ), _body( ::btRigidBody::btRigidBodyConstructionInfo( 0.0f, nullptr, &_collisionShape ) ) {
		owningView._dynamicsWorld.addCollisionObject( &_body, TerrainCollider::CollisionFilterGroup, TerrainCollider::CollisionFilterMask );
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::TerrainCollider::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void WorldView::TerrainCollider::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeCollisionObject( &_body );
		worldView.GetWorldAllocator().Delete( *this, ::Eldritch2::AlignedDeallocationSemantics );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2