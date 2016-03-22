/*==================================================================*\
  WorldView.PhysicalArmature.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Physics/PhysX/WorldView.hpp>
#include <Animation/KeyCache.hpp>
//------------------------------------------------------------------//
#include <PxAggregate.h>
#include <PxPhysics.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	const char* const WorldView::PhysicalArmature::scriptTypeName = "PhysicalArmature";

// ---------------------------------------------------

	WorldView::PhysicalArmature::PhysicsAnimationLayer::PhysicsAnimationLayer( PhysX::UniquePointer<PxAggregate>&& aggregate ) : _aggregate( ::std::move( aggregate ) ) {}

// ---------------------------------------------------

	WorldView::PhysicalArmature::PhysicsAnimationLayer::~PhysicsAnimationLayer() {}

// ---------------------------------------------------

	void WorldView::PhysicalArmature::PhysicsAnimationLayer::EvaluateCacheForTime( KeyCache& /*keyCache*/, BoneIndex /*maximumBoneToConsider*/, uint64 /*time*/ ) {
	}

// ---------------------------------------------------

	void WorldView::PhysicalArmature::PhysicsAnimationLayer::Dispose() {}

// ---------------------------------------------------

	WorldView::PhysicalArmature::PhysicalArmature( PhysX::UniquePointer<PxAggregate>&& aggregate ) : _physicsAnimationLayer( ::std::move( aggregate ) ) {
		AttachLayer( _physicsAnimationLayer );
	}

// ---------------------------------------------------

	WorldView::PhysicalArmature::~PhysicalArmature() {
		DetachLayer( _physicsAnimationLayer );
	}

// ---------------------------------------------------

	void WorldView::PhysicalArmature::Dispose() {
		GetActiveWorldView()._armatureAllocator.Delete( *this );
	}

// ---------------------------------------------------

	ETNoAliasHint void WorldView::PhysicalArmature::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		auto	builder( typeRegistrar.BeginReferenceTypeRegistration<PhysicalArmature>() );

		// Factory registration
		builder.ExposeFactory<const RigidTransformMarshal&, const StringMarshal&>( [] ( const RigidTransformMarshal& transform, const StringMarshal& resourceName ) -> PhysicalArmature* {
			const auto	resource( GetActiveWorldView().GetContentLibrary().ResolveViewByName<FileSystem::ResourceView>( resourceName.AsCString() ) );

			if( !resource ) {
				return nullptr;
			}

			PhysX::UniquePointer<PxAggregate>	aggregate( PxGetPhysics().createAggregate( 70u, false ) );

			return new(GetActiveWorldView()._armatureAllocator, Allocator::AllocationDuration::Normal) PhysicalArmature( ::std::move( aggregate ) );
		} );

		// Reference cast registration
		builder.ExposeReferenceCast<Armature>( [] ( PhysicalArmature* armature ) {
			return static_cast<Armature*>(armature);
		} ).ExposeReferenceCast<const Armature>( [] ( const PhysicalArmature* armature ) {
			return static_cast<const Armature*>(armature);
		} );
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2
