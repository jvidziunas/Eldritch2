/*==================================================================*\
  WorldView.TriggerVolume.cpp
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
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	const char* const WorldView::TriggerVolume::scriptTypeName = UTF8L("TriggerVolume");

// ---------------------------------------------------

	WorldView::TriggerVolume::TriggerVolume( WorldView& owningWorldView ) : _ghostObject() {
		owningWorldView._dynamicsWorld.addCollisionObject( &_ghostObject, TriggerVolume::CollisionFilterGroup, TriggerVolume::CollisionFilterMask );
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::TriggerVolume::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& /*typeRegistrar*/ ) {}

// ---------------------------------------------------

	void WorldView::TriggerVolume::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeCollisionObject( &_ghostObject );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2