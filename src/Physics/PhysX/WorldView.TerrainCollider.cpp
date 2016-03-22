/*==================================================================*\
  WorldView.TerrainCollider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Physics/PhysX/TerrainResourceView.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <PxRigidStatic.h>
#include <PxPhysics.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const WorldView::TerrainCollider::scriptTypeName	= "TerrainCollider";

// ---------------------------------------------------

	WorldView::TerrainCollider::TerrainCollider( UniquePointer<PxRigidStatic>&& actor, const PhysX::TerrainResourceView& asset ) : _actor( ::std::move( actor ) ), _asset( &asset ), _enabled( false ) {
		ETRuntimeAssert( nullptr != _actor );

		_actor->userData = this;
	}

// ---------------------------------------------------

	WorldView::TerrainCollider::~TerrainCollider() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::TerrainCollider::SetEnabled( bool enabled ) {
		if( _enabled == enabled ) {
			return;
		}

		_enabled = enabled;

		if( enabled ) {
			_actor->attachShape( _asset->GetShape() );
		} else {
			_actor->detachShape( _asset->GetShape() );
		}
	}

// ---------------------------------------------------

	bool WorldView::TerrainCollider::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::TerrainCollider::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		auto	builder( typeRegistrar.BeginReferenceTypeRegistration<TerrainCollider>() );

		builder.ExposeFactory<const Float4Marshal&, const StringMarshal&>( [] ( const Float4Marshal& position, const StringMarshal& resourceName ) -> TerrainCollider* {
			const auto	resourceView( GetActiveWorldView().GetContentLibrary().ResolveViewByName<TerrainResourceView>( resourceName.AsCString() ) );

			if( !resourceView ) {
				return nullptr;
			}

			if( UniquePointer<PxRigidStatic> physicsObject { PxGetPhysics().createRigidStatic( { position.coefficients[0], position.coefficients[1], position.coefficients[2] } ) } ) {
				GetActiveWorldView().GetScene().addActor( *physicsObject );

				return new(GetActiveWorldView()._componentAllocator, Allocator::AllocationDuration::Normal) TerrainCollider( ::std::move( physicsObject ), *resourceView );
			}

			return nullptr;
		} );

		builder.ExposeVirtualProperty<bool>( "Enabled", [] ( TerrainCollider* collider, bool value ) {
			collider->SetEnabled( value );
		} ).ExposeVirtualProperty<bool>( "Enabled", [] ( TerrainCollider* collider ) {
			return collider->GetEnabled();
		} );
	}

// ---------------------------------------------------

	void WorldView::TerrainCollider::Dispose() {
		GetActiveWorldView()._componentAllocator.Delete( *this );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

