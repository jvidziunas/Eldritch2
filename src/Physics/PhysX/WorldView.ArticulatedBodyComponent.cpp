/*==================================================================*\
  WorldView.ArticulatedBodyComponent.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/PhysX/ArticulatedBodyResourceView.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <PxArticulation.h>
#include <PxScene.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const WorldView::ArticulatedBodyComponent::scriptTypeName = "ArticulatedBody";

// ---------------------------------------------------

	WorldView::ArticulatedBodyComponent::ArticulatedBodyComponent( UniquePointer<PxArticulation>&& articulation ) : _articulation( ::std::move( articulation ) ), _enabled( false ) {
		SetEnabled( true );
	}

// ---------------------------------------------------

	WorldView::ArticulatedBodyComponent::~ArticulatedBodyComponent() {
		SetEnabled( false );
	}

// ---------------------------------------------------

	void WorldView::ArticulatedBodyComponent::SetEnabled( bool value ) {
		if( _enabled == value ) {
			return;
		}

		_enabled = value;

		if( value ) {
			GetActiveWorldView().GetScene().addArticulation( *_articulation );
		} else {
			GetActiveWorldView().GetScene().removeArticulation( *_articulation );
		}
	}

// ---------------------------------------------------

	bool WorldView::ArticulatedBodyComponent::GetEnabled() const {
		return _enabled;
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::ArticulatedBodyComponent::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		FixedStackAllocator<64u>	allocator( UTF8L("ArticulatedBodyComponent::ExposeScriptAPI() Temporary Allocator") );
		auto						typeBuilderResult( typeRegistrar.RegisterUserDefinedReferenceType<ArticulatedBodyComponent>( allocator ) );

		ETRuntimeAssert( typeBuilderResult );

		auto&	typeBuilder( *typeBuilderResult.object );

		typeBuilder.ExposeVirtualProperty( "Enabled", &ArticulatedBodyComponent::SetEnabled ).ExposeVirtualProperty( "Enabled", &ArticulatedBodyComponent::GetEnabled );

		allocator.Delete( typeBuilder );
	}

// ---------------------------------------------------

	void WorldView::ArticulatedBodyComponent::Dispose() {}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

