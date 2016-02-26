/*==================================================================*\
  WorldView.AnimationDrivenCharacterCollider.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxCapsuleController.h>
#include <characterkinematic/PxControllerManager.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const WorldView::AnimationDrivenCharacterCollider::scriptTypeName	= "AnimationDrivenCharacterCollider";

// ---------------------------------------------------

	WorldView::AnimationDrivenCharacterCollider::AnimationDrivenCharacterCollider( UniquePointer<PxCapsuleController>&& controller ) : _controller( ::std::move( controller ) ) {}

// ---------------------------------------------------

	WorldView::AnimationDrivenCharacterCollider::~AnimationDrivenCharacterCollider() {}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::AnimationDrivenCharacterCollider::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static AnimationDrivenCharacterCollider* ETScriptAPICall Factory0( const Float4Marshal& position, float32 radius, float32 height ) {
				PxCapsuleControllerDesc		descriptor;

				descriptor.position = PxExtendedVec3( position.coefficients[0], position.coefficients[1], position.coefficients[2] );
				descriptor.height	= height;
				descriptor.radius	= radius;

				if( UniquePointer<PxCapsuleController>	controller { static_cast<PxCapsuleController*>(GetActiveWorldView().GetControllerManager().createController( descriptor )) } ) {
					return new(GetActiveWorldView()._componentAllocator, Allocator::AllocationDuration::Normal) AnimationDrivenCharacterCollider( ::std::move( controller ) );
				}

				return nullptr;
			}
		};

	// ---

		FixedStackAllocator<64u>	allocator( UTF8L("AnimationDrivenCharacterCollider::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<AnimationDrivenCharacterCollider>( allocator ) );
		auto&						typeBuilder( *typeBuilderResult.object );

		ETRuntimeAssert( typeBuilderResult );

		typeBuilder.ExposeFactory( &FunctionHelper::Factory0 );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::AnimationDrivenCharacterCollider::Dispose() {
		GetActiveWorldView()._componentAllocator.Delete( *this );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2