/*==================================================================*\
  WorldView.CharacterControllerComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxController.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const WorldView::CharacterControllerComponent::scriptTypeName	= "CharacterController";

// ---------------------------------------------------

	WorldView::CharacterControllerComponent::CharacterControllerComponent( UniquePointer<PxController>&& controller ) : _controller( ::std::move( controller ) ) {}

// ---------------------------------------------------

	WorldView::CharacterControllerComponent::~CharacterControllerComponent() {}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::CharacterControllerComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<64u>	allocator( UTF8L("CharacterControllerComponent::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<CharacterControllerComponent>( allocator ) );

		ETRuntimeAssert( typeBuilderResult );

		auto&	typeBuilder( *typeBuilderResult.object );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::CharacterControllerComponent::Dispose() {}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2