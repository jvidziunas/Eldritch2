/*==================================================================*\
  WorldView.TriggerVolumeComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
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

	const char* const	WorldView::TriggerVolumeComponent::scriptTypeName = "TriggerVolume";

// ---------------------------------------------------

	WorldView::TriggerVolumeComponent::TriggerVolumeComponent( UniquePointer<PxRigidStatic>&& actor ) : _actor( ::std::move( actor ) ), _enabled( false ) {
		SetEnabled( true );
	}

// ---------------------------------------------------

	WorldView::TriggerVolumeComponent::~TriggerVolumeComponent() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::TriggerVolumeComponent::SetEnabled( bool value ) {
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

	bool WorldView::TriggerVolumeComponent::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::TriggerVolumeComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<64u>	allocator( UTF8L("TriggerVolumeComponent::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<TriggerVolumeComponent>( allocator ) );

		ETRuntimeAssert( typeBuilderResult );

		auto&	typeBuilder( *typeBuilderResult.object );

		typeBuilder.ExposeVirtualProperty( "Enabled", &TriggerVolumeComponent::SetEnabled ).ExposeVirtualProperty( "Enabled", &TriggerVolumeComponent::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::TriggerVolumeComponent::Dispose() {}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2



