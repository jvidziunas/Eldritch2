/*==================================================================*\
  WorldView.TerrainColliderComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Utility/Assert.hpp>
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

	const char* const WorldView::TerrainColliderComponent::scriptTypeName	= "TerrainCollider";

// ---------------------------------------------------

	WorldView::TerrainColliderComponent::TerrainColliderComponent( UniquePointer<PxRigidStatic>&& actor ) : _actor( ::std::move( actor ) ) {
		ETRuntimeAssert( nullptr != _actor );

		_actor->userData = this;
	}

// ---------------------------------------------------

	WorldView::TerrainColliderComponent::~TerrainColliderComponent() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::TerrainColliderComponent::SetEnabled( bool value ) {
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

	bool WorldView::TerrainColliderComponent::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::TerrainColliderComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<64u>	allocator( UTF8L( "PhysicalSoftBodyComponent::ExposeScriptAPI() Temporary Allocator" ) );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<TerrainColliderComponent>( allocator ) );

		ETRuntimeAssert( typeBuilderResult );

		auto&	typeBuilder( *typeBuilderResult.object );

		typeBuilder.ExposeVirtualProperty( "Enabled", &TerrainColliderComponent::SetEnabled ).ExposeVirtualProperty( "Enabled", &TerrainColliderComponent::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::TerrainColliderComponent::Dispose() {
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

