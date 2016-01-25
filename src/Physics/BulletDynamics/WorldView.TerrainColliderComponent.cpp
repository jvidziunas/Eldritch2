/*==================================================================*\
  WorldView.TerrainColliderComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	const char* const WorldView::TerrainColliderComponent::scriptTypeName = UTF8L("TerrainCollider");

// ---------------------------------------------------

	WorldView::TerrainColliderComponent::TerrainColliderComponent( WorldView& owningView ) : _collisionShape( 0, 0, nullptr, 0.0f, 0.0f, 0.0f, 0, PHY_FLOAT, false ), _body( ::btRigidBody::btRigidBodyConstructionInfo( 0.0f, nullptr, &_collisionShape ) ) {
		owningView._dynamicsWorld.addCollisionObject( &_body, TerrainColliderComponent::CollisionFilterGroup, TerrainColliderComponent::CollisionFilterMask );
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::TerrainColliderComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static TerrainColliderComponent* ETScriptAPICall Factory0( const StringMarshal& /*resourceName*/ ) {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(TerrainColliderComponent), Allocator::AllocationDuration::Normal) TerrainColliderComponent( worldView );
			}

			static void ETScriptAPICall SetIsEnabled( TerrainColliderComponent* /*component*/, bool /*enabled*/ ) {}

			static bool ETScriptAPICall GetIsEnabled( const TerrainColliderComponent* /*component*/ ) {
				return true;
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::TerrainColliderComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<TerrainColliderComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
			typeBuilder.ExposeVirtualProperty( "IsEnabled", &FunctionHelper::SetIsEnabled ).ExposeVirtualProperty( "IsEnabled", &FunctionHelper::GetIsEnabled );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::TerrainColliderComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeCollisionObject( &_body );
		worldView.GetWorldAllocator().Delete( *this, ::Eldritch2::AlignedDeallocationSemantics );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2