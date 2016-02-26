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
#include <Utility/Assert.hpp>
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
		struct FunctionHelper {
			static MeshCollider* ETScriptAPICall Factory0( PhysicalArmature& armature, const StringMarshal& resourceName ) {
				const auto	resource( GetActiveWorldView().GetContentLibrary().ResolveViewByName<MeshResourceView>( resourceName.GetCharacterArray() ) );

				if( !resource ) {
					return nullptr;
				}

				return new(GetActiveWorldView()._componentAllocator, Allocator::AllocationDuration::Normal) MeshCollider( armature, *resource );
			}
		};

	// ---

		FixedStackAllocator<64u>	allocator( UTF8L("MeshCollider::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<MeshCollider>( allocator ) );
		auto&						typeBuilder( *typeBuilderResult.object );

		ETRuntimeAssert( typeBuilderResult );

		typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
		typeBuilder.ExposeVirtualProperty( "Enabled", &MeshCollider::SetEnabled ).ExposeVirtualProperty( "Enabled", &MeshCollider::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::MeshCollider::Dispose() {
		GetActiveWorldView()._componentAllocator.Delete( *this );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

