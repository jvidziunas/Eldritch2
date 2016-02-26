/*==================================================================*\
  WorldView.TriggerVolume.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
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

	const char* const WorldView::TriggerVolume::scriptTypeName = UTF8L("TriggerVolume");

// ---------------------------------------------------

	WorldView::TriggerVolume::TriggerVolume( WorldView& owningWorldView ) : _ghostObject() {
		owningWorldView._dynamicsWorld.addCollisionObject( &_ghostObject, TriggerVolume::CollisionFilterGroup, TriggerVolume::CollisionFilterMask );
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::TriggerVolume::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static TriggerVolume* Factory0() {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(TriggerVolume), Allocator::AllocationDuration::Normal) TriggerVolume( worldView );
			}

			static void SetIsEnabled( TriggerVolume* /*component*/, bool /*enabled*/ ) {}

			static bool GetIsEnabled( const TriggerVolume* /*component*/ ) {
				return true;
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L( "WorldView::TriggerVolume::ExposeScriptAPI() Temporary Allocator" ) );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<TriggerVolume>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
			typeBuilder.ExposeVirtualProperty( "IsEnabled", &FunctionHelper::SetIsEnabled ).ExposeVirtualProperty( "IsEnabled", &FunctionHelper::GetIsEnabled );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::TriggerVolume::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeCollisionObject( &_ghostObject );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2