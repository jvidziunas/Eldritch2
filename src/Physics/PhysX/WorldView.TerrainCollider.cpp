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
		struct FunctionHelper {
			static TerrainCollider* ETScriptAPICall Factory0( const Float4Marshal& position, const StringMarshal& resourceName ) {
				const auto	resourceView( GetActiveWorldView().GetContentLibrary().ResolveViewByName<TerrainResourceView>( resourceName.GetCharacterArray() ) );

				if( !resourceView ) {
					return nullptr;
				}

				if( UniquePointer<PxRigidStatic> physicsObject { PxGetPhysics().createRigidStatic( { position.coefficients[0], position.coefficients[1], position.coefficients[2] } ) } ) {
					GetActiveWorldView().GetScene().addActor( *physicsObject );

					return new(GetActiveWorldView()._componentAllocator, Allocator::AllocationDuration::Normal) TerrainCollider( ::std::move( physicsObject ), *resourceView );
				}

				return nullptr;
			}
		};

	// ---

		FixedStackAllocator<64u>	allocator( UTF8L("TerrainCollider::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<TerrainCollider>( allocator ) );
		auto&						typeBuilder( *typeBuilderResult.object );

		ETRuntimeAssert( typeBuilderResult );

		typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
		typeBuilder.ExposeVirtualProperty( "Enabled", &TerrainCollider::SetEnabled ).ExposeVirtualProperty( "Enabled", &TerrainCollider::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::TerrainCollider::Dispose() {
		GetActiveWorldView()._componentAllocator.Delete( *this );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

