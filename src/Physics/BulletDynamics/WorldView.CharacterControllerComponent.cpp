/*==================================================================*\
  WorldView.CharacterControllerComponent.cpp
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

	const char* const WorldView::CharacterControllerComponent::scriptTypeName = UTF8L("CharacterController");

// ---------------------------------------------------

	WorldView::CharacterControllerComponent::CharacterControllerComponent( WorldView& owningWorldView ) : _ghostObject(), _controller( &_ghostObject, nullptr, 0.1f ) {}

// ---------------------------------------------------

	WorldView::CharacterControllerComponent::~CharacterControllerComponent() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::CharacterControllerComponent::SetEnabled( bool value /*= true */ ) {
		auto&	dynamicsWorld( GetActiveWorldView()._dynamicsWorld );

		if( _enabled == value ) {
			return;
		}

		_enabled = value;

		if( value ) {
			dynamicsWorld.addCollisionObject( &_ghostObject, CharacterControllerComponent::CollisionFilterGroup, CharacterControllerComponent::CollisionFilterMask );
			dynamicsWorld.addAction( &_controller );
		} else {
			dynamicsWorld.removeAction( &_controller );
			dynamicsWorld.removeCollisionObject( &_ghostObject );
		}
	}

// ---------------------------------------------------

	bool WorldView::CharacterControllerComponent::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::CharacterControllerComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static CharacterControllerComponent* ETScriptAPICall Factory0( const StringMarshal& /*resourceName*/ ) {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(CharacterControllerComponent), Allocator::AllocationDuration::Normal) CharacterControllerComponent( worldView );
			}

			static bool ETScriptAPICall IsOnGround( const CharacterControllerComponent* armature ) {
				return armature->_controller.onGround();
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::CharacterControllerComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<CharacterControllerComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureReferenceTypeDeclared<Armature>();
			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );
			typeBuilder.ExposeVirtualProperty( "IsOnGround", &FunctionHelper::IsOnGround );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::CharacterControllerComponent::Dispose() {
		GetActiveWorldView().GetWorldAllocator().Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2