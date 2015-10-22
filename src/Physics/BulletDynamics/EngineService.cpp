/*==================================================================*\
  EngineService.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceViewFactoryPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/BulletDynamics/CollisionShapeResourceView.hpp>
#include <Physics/BulletDynamics/EngineService.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _persistentManifoldPoolSizeInElements( 4096u ), _collisionAlgorithmPoolSizeInElements( 4096u ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Bullet Dynamics Engine");
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		using AllocationOption	= Allocator::AllocationOption;
		using Initializer		= ResourceView::Initializer;

	// ---

		// Collision shape view.
		visitor.PublishFactory( CollisionShapeResourceView::GetSerializedDataTag(), this, [] ( Allocator& allocator, const Initializer& initializer, void* /*engine*/ ) -> Result<ResourceView> {
			unique_ptr<CollisionShapeResourceView, InstanceDeleter>	view( new(allocator, AllocationOption::PERMANENT_ALLOCATION) CollisionShapeResourceView( initializer, allocator ), { allocator } );

			if( view && view->InstantiateFromByteArray( initializer.serializedAsset ) ) {
				return { *view.release() };
			}

			return { view ? Error::INVALID_PARAMETER : Error::OUT_OF_MEMORY };
		} );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("BulletDynamics") );

		visitor.Register( UTF8L("WorldPersistentManifoldPoolSizeInElements"), _persistentManifoldPoolSizeInElements );
		visitor.Register( UTF8L("WorldCollisionAlgorithmPoolSizeInElements"), _collisionAlgorithmPoolSizeInElements );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		WorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		// Reserve a little extra slack here for the aligned allocation.
		visitor.PublishFactory( this, sizeof(WorldView) + (alignof(WorldView) - 1u), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			// Important to use aligned allocation. Visual Studio uses a movaps instruction for the compiler-generated btVector3::operator=() method, and it's quite common for
			// one of the vectors to end up on an unaligned boundary.
			return new(allocator, alignof(WorldView), Allocator::AllocationOption::PERMANENT_ALLOCATION) WorldView( world, *static_cast<EngineService*>(parameter) ) ? Error::NONE : Error::OUT_OF_MEMORY;
		} );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2