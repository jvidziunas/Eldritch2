/*==================================================================*\
  WorldView.AnimationDrivenCharacterCollider.cpp
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
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/MPL/VectorTypes.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	const char* const WorldView::AnimationDrivenCharacterCollider::scriptTypeName = "AnimationDrivenCharacterCollider";

// ---------------------------------------------------

	WorldView::AnimationDrivenCharacterCollider::AnimationDrivenCharacterCollider( WorldView& owningWorldView ) : _ghostObject(), _controller( &_ghostObject, nullptr, 0.1f ) {}

// ---------------------------------------------------

	WorldView::AnimationDrivenCharacterCollider::~AnimationDrivenCharacterCollider() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::AnimationDrivenCharacterCollider::SetEnabled( bool value /*= true */ ) {
		auto&	dynamicsWorld( GetActiveWorldView()._dynamicsWorld );

		if( _enabled == value ) {
			return;
		}

		_enabled = value;

		if( value ) {
			dynamicsWorld.addCollisionObject( &_ghostObject, AnimationDrivenCharacterCollider::CollisionFilterGroup, AnimationDrivenCharacterCollider::CollisionFilterMask );
			dynamicsWorld.addAction( &_controller );
		} else {
			dynamicsWorld.removeAction( &_controller );
			dynamicsWorld.removeCollisionObject( &_ghostObject );
		}
	}

// ---------------------------------------------------

	bool WorldView::AnimationDrivenCharacterCollider::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::AnimationDrivenCharacterCollider::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static AnimationDrivenCharacterCollider* ETScriptAPICall Factory0( const StringMarshal& /*resourceName*/ ) {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(AnimationDrivenCharacterCollider), Allocator::AllocationDuration::Normal) AnimationDrivenCharacterCollider( worldView );
			}

			static bool ETScriptAPICall IsOnGround( const AnimationDrivenCharacterCollider* armature ) {
				return armature->_controller.onGround();
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::AnimationDrivenCharacterCollider::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<AnimationDrivenCharacterCollider>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureReferenceTypeDeclared<Armature>();
			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
			typeBuilder.ExposeVirtualProperty( "IsOnGround", &FunctionHelper::IsOnGround );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::AnimationDrivenCharacterCollider::Dispose() {
		GetActiveWorldView().GetWorldAllocator().Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2