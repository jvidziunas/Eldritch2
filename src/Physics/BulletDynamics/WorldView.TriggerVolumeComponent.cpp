/*==================================================================*\
  WorldView.TriggerVolumeComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	const char* const WorldView::TriggerVolumeComponent::scriptTypeName = UTF8L("TriggerVolume");

// ---------------------------------------------------

	WorldView::TriggerVolumeComponent::TriggerVolumeComponent( WorldView& owningWorldView ) : _ghostObject() {
		owningWorldView._dynamicsWorld.addCollisionObject( &_ghostObject, COLLISION_FILTER_GROUP, COLLISION_FILTER_MASK );
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::TriggerVolumeComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static TriggerVolumeComponent* Factory0() {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(TriggerVolumeComponent), Allocator::AllocationOption::PERMANENT_ALLOCATION) TriggerVolumeComponent( worldView );
			}

			static void SetIsEnabled( TriggerVolumeComponent* /*component*/, bool /*enabled*/ ) {}

			static bool GetIsEnabled( const TriggerVolumeComponent* /*component*/ ) {
				return true;
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L( "WorldView::TriggerVolumeComponent::ExposeScriptAPI() Temporary Allocator" ) );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<TriggerVolumeComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
			typeBuilder.ExposeVirtualProperty( "IsEnabled", &FunctionHelper::SetIsEnabled ).ExposeVirtualProperty( "IsEnabled", &FunctionHelper::GetIsEnabled );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::TriggerVolumeComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeCollisionObject( &_ghostObject );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2