/*==================================================================*\
  WorldView.TriggerVolume.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Physics/PhysX/WorldView.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const	WorldView::TriggerVolume::scriptTypeName = "TriggerVolume";

// ---------------------------------------------------

	WorldView::TriggerVolume::TriggerVolume( UniquePointer<PxRigidStatic>&& actor ) : _actor( ::std::move( actor ) ), _enabled( false ) {
		SetEnabled( true );
	}

// ---------------------------------------------------

	WorldView::TriggerVolume::~TriggerVolume() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::TriggerVolume::SetEnabled( bool value ) {
		if( _enabled == value ) {
			return;
		}

		_enabled = value;

		if( value ) {
			GetActiveWorldView().GetScene().addActor( *_actor );
		} else {
			GetActiveWorldView().GetScene().removeActor( *_actor );
		}
	}

// ---------------------------------------------------

	bool WorldView::TriggerVolume::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::TriggerVolume::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<64u>	allocator( UTF8L("TriggerVolume::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<TriggerVolume>( allocator ) );

		ETRuntimeAssert( typeBuilderResult );

		auto&	typeBuilder( *typeBuilderResult.object );

		typeBuilder.ExposeVirtualProperty( "Enabled", &TriggerVolume::SetEnabled ).ExposeVirtualProperty( "Enabled", &TriggerVolume::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::TriggerVolume::Dispose() {}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



