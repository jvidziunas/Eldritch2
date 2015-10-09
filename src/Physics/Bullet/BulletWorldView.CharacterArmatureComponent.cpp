/*==================================================================*\
  BulletWorldView.CharacterArmatureComponent.cpp
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

	const char* const BulletWorldView::CharacterArmatureComponent::scriptTypeName = UTF8L("CharacterArmature");

// ---------------------------------------------------

	BulletWorldView::CharacterArmatureComponent::CharacterArmatureComponent( BulletWorldView& owningWorldView ) : _ghostObject(), _controller( &_ghostObject, nullptr, 0.1f ) {
		owningWorldView._dynamicsWorld.addCollisionObject( &_ghostObject, COLLISION_FILTER_GROUP, COLLISION_FILTER_MASK );
		owningWorldView._dynamicsWorld.addAction( &_controller );
	}

// ---------------------------------------------------

	BulletWorldView::CharacterArmatureComponent::~CharacterArmatureComponent() {}

// ---------------------------------------------------

	void ETNoAliasHint BulletWorldView::CharacterArmatureComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static CharacterArmatureComponent* ETScriptAPICall Factory0( const StringMarshal& /*resourceName*/ ) {
				auto&	worldView( GetActiveWorldView() );

				return new(worldView.GetWorldAllocator(), alignof(CharacterArmatureComponent), Allocator::AllocationOption::PERMANENT_ALLOCATION) CharacterArmatureComponent( worldView );
			}

			static bool ETScriptAPICall IsOnGround( const CharacterArmatureComponent* armature ) {
				return armature->_controller.onGround();
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("BulletWorldView::CharacterArmatureComponent::ExposeScriptAPI() Temporary Allocator") );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedReferenceType<CharacterArmatureComponent>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureReferenceTypeDeclared<Armature>();
			typeBuilder.ExposeFactory( &FunctionHelper::Factory0 ).ExposeReferenceCastTo<Armature>();
			typeBuilder.ExposeVirtualProperty( "IsOnGround", &FunctionHelper::IsOnGround );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void BulletWorldView::CharacterArmatureComponent::Dispose() {
		auto&	worldView( GetActiveWorldView() );

		worldView._dynamicsWorld.removeAction( &_controller );
		worldView.GetWorldAllocator().Delete( *this );
	}

}	// namespace Physics
}	// namespace Eldritch2