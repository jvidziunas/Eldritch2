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
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Physics/BulletDynamics/MeshResourceView.hpp>
#include <Physics/BulletDynamics/EngineService.hpp>
#include <Physics/BulletDynamics/WorldView.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	EngineService::EngineService( GameEngine& owningEngine ) : GameEngineService( owningEngine ), _persistentManifoldPoolSizeInElements( 4096u ), _collisionAlgorithmPoolSizeInElements( 4096u ) {}

// ---------------------------------------------------

	const UTF8Char* const EngineService::GetName() const {
		return UTF8L("Bullet Dynamics Engine");
	}

// ---------------------------------------------------

	ErrorCode EngineService::AllocateWorldView( Allocator& allocator, World& world ) {
		return new(allocator, alignof(WorldView), Allocator::AllocationDuration::Normal) WorldView( world, *this ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ResourceViewFactoryPublishingInitializationVisitor& visitor ) {
		visitor.PublishFactory( MeshResourceView::GetSerializedDataTag(), _meshFactory );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("BulletDynamics") );

		visitor.Register( UTF8L("WorldPersistentManifoldPoolSizeInElements"), _persistentManifoldPoolSizeInElements );
		visitor.Register( UTF8L("WorldCollisionAlgorithmPoolSizeInElements"), _collisionAlgorithmPoolSizeInElements );

		_meshFactory.AcceptInitializationVisitor( visitor );
	}

// ---------------------------------------------------

	void EngineService::AcceptInitializationVisitor( ScriptApiRegistrationInitializationVisitor& visitor ) {
		WorldView::ExposeScriptAPI( visitor );
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2