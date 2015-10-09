/*==================================================================*\
  BulletWorldView.TriggerVolumeComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/Bullet/BulletWorldView.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {

	const char* const BulletWorldView::TriggerVolumeComponent::scriptTypeName = UTF8L("TriggerVolume");

// ---------------------------------------------------

	BulletWorldView::TriggerVolumeComponent::TriggerVolumeComponent( BulletWorldView& owningWorldView ) : _ghostObject() {
		owningWorldView._dynamicsWorld.addCollisionObject( &_ghostObject, COLLISION_FILTER_GROUP, COLLISION_FILTER_MASK );
	}

// ---------------------------------------------------

	BulletWorldView::TriggerVolumeComponent::~TriggerVolumeComponent() {}

// ---------------------------------------------------

	void ETNoAliasHint BulletWorldView::TriggerVolumeComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
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

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L( "BulletWorldView::TriggerVolumeComponent::ExposeScriptAPI() Temporary Allocator" ) );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<TriggerVolumeComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
			typeBuilder.ExposeVirtualProperty( "IsEnabled", &FunctionHelper::SetIsEnabled ).ExposeVirtualProperty( "IsEnabled", &FunctionHelper::GetIsEnabled );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void BulletWorldView::TriggerVolumeComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeCollisionObject( &_ghostObject );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace Physics
}	// namespace Eldritch2