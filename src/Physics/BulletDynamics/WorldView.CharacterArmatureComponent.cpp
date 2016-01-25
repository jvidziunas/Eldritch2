/*==================================================================*\
  WorldView.CharacterArmatureComponent.cpp
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

	const char* const WorldView::CharacterArmatureComponent::scriptTypeName = UTF8L("CharacterArmature");

// ---------------------------------------------------

	WorldView::CharacterArmatureComponent::CharacterArmatureComponent( WorldView& owningWorldView ) : _ghostObject(), _controller( &_ghostObject, nullptr, 0.1f ) {
		owningWorldView._dynamicsWorld.addCollisionObject( &_ghostObject, CharacterArmatureComponent::CollisionFilterGroup, CharacterArmatureComponent::CollisionFilterMask );
		owningWorldView._dynamicsWorld.addAction( &_controller );
	}

// ---------------------------------------------------

	void ETNoAliasHint WorldView::CharacterArmatureComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static CharacterArmatureComponent* ETScriptAPICall Factory0( const StringMarshal& /*resourceName*/ ) {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(CharacterArmatureComponent), Allocator::AllocationDuration::Normal) CharacterArmatureComponent( worldView );
			}

			static bool ETScriptAPICall IsOnGround( const CharacterArmatureComponent* armature ) {
				return armature->_controller.onGround();
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("WorldView::CharacterArmatureComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<CharacterArmatureComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureReferenceTypeDeclared<Armature>();
			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 ).ExposeReferenceCastTo<Armature>();
			typeBuilder.ExposeVirtualProperty( "IsOnGround", &FunctionHelper::IsOnGround );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void WorldView::CharacterArmatureComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeAction( &_controller );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2