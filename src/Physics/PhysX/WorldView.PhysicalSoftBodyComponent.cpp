/*==================================================================*\
  WorldView.PhysicalSoftBodyComponent.cpp
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
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <cloth/PxCloth.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const	WorldView::PhysicalSoftBodyComponent::scriptTypeName = "PhysicalSoftBody";

// ---------------------------------------------------

	WorldView::PhysicalSoftBodyComponent::PhysicalSoftBodyComponent( UniquePointer<PxCloth>&& actor ) : _actor( ::std::move( actor ) ), _enabled( false ) {
		SetEnabled( true );
	}

// ---------------------------------------------------

	WorldView::PhysicalSoftBodyComponent::~PhysicalSoftBodyComponent() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::PhysicalSoftBodyComponent::SetEnabled( bool value ) {
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

	bool WorldView::PhysicalSoftBodyComponent::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::PhysicalSoftBodyComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<64u>	allocator( UTF8L("PhysicalSoftBodyComponent::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<PhysicalSoftBodyComponent>( allocator ) );

		ETRuntimeAssert( typeBuilderResult );

		auto&	typeBuilder( *typeBuilderResult.object );

		typeBuilder.ExposeVirtualProperty( "Enabled", &PhysicalSoftBodyComponent::SetEnabled ).ExposeVirtualProperty( "Enabled", &PhysicalSoftBodyComponent::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::PhysicalSoftBodyComponent::Dispose() {

	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2