/*==================================================================*\
  WorldView.MeshCollider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Physics/PhysX/MeshResourceView.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Physics/PhysX/WorldView.hpp>
//------------------------------------------------------------------//
#include <PxRigidActor.h>
#include <PxAggregate.h>
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

	const char* const WorldView::MeshCollider::scriptTypeName = "MeshCollider";

// ---------------------------------------------------

	WorldView::MeshCollider::MeshCollider( WorldView::PhysicalArmature& armature, const MeshResourceView& asset ) : _armature( armature ), _asset( &asset ), _enabled( false ) {
		SetEnabled( true );
	}

// ---------------------------------------------------

	WorldView::MeshCollider::~MeshCollider() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::MeshCollider::SetEnabled( bool enabled ) {
		if( _enabled == enabled ) {
			return;
		}

		_enabled = enabled;

		auto&		aggregate( _armature->GetAggregate() );
		const auto	actorCount( aggregate.getNbActors() );
		const auto	actors( static_cast<PxActor**>(_alloca( sizeof(PxActor*) * actorCount )) );

		aggregate.getActors( actors, actorCount );

		if( enabled ) {
			for( const auto& definition : _asset->GetRigidBodies() ) {
				static_cast<PxRigidActor*>(actors[definition.GetBodyIndex()])->attachShape( definition.GetShape() );
			}
		} else {
			for( const auto& definition : _asset->GetRigidBodies() ) {
				static_cast<PxRigidActor*>(actors[definition.GetBodyIndex()])->detachShape( definition.GetShape() );
			}
		}
	}

// ---------------------------------------------------

	bool WorldView::MeshCollider::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::MeshCollider::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		auto	builder( typeRegistrar.BeginReferenceTypeRegistration<MeshCollider>() );

		builder.ExposeFactory<PhysicalArmature*, const StringMarshal&>( [] ( PhysicalArmature* armature, const StringMarshal& resourceName ) -> MeshCollider* {
			const auto	resource( GetActiveWorldView().GetContentLibrary().ResolveViewByName<MeshResourceView>( resourceName.AsCString() ) );

			if( !resource || !armature ) {
				return nullptr;
			}

			return new(GetActiveWorldView()._componentAllocator, Allocator::AllocationDuration::Normal) MeshCollider( *armature, *resource );
		} );
		
		builder.ExposeVirtualProperty<bool>( "Enabled", [] ( MeshCollider* collider, bool value ) {
			collider->SetEnabled( value );
		} ).ExposeVirtualProperty<bool>( "Enabled", [] ( const MeshCollider* collider ) {
			return collider->GetEnabled();
		} );
	}

// ---------------------------------------------------

	void WorldView::MeshCollider::Dispose() {
		GetActiveWorldView()._componentAllocator.Delete( *this );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

