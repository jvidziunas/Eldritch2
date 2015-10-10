/*==================================================================*\
  BulletEngine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.hpp>
#include <Configuration/ConfigurationPublishingInitializationVisitor.hpp>
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Physics/Bullet/BulletWorldView.hpp>
#include <Physics/BulletEngine.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {

	BulletCollisionShapeView::BulletCollisionShapeView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ),
																												 _shapeAllocator( allocator, UTF8L("Bullet Collision Shape Allocator") ),
																												 _shapes( 0u, allocator, UTF8L("Bullet Collision Shape View Subshape Collection Allocator") ) {}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const BulletCollisionShapeView::GetSerializedDataTag() {
		return UTF8L("CollisionShape");
	}

// ---------------------------------------------------

	ErrorCode BulletCollisionShapeView::DeserializeFromPackageExport( Allocator& allocator, const Initializer& initializer, BulletEngine& /*physicsEngine*/ ) {
		if( auto* const view = new(allocator, Allocator::AllocationOption::PERMANENT_ALLOCATION) BulletCollisionShapeView( initializer, allocator ) ) {
			return Errors::NONE;
		}

		return Errors::OUT_OF_MEMORY;
	}

// ---------------------------------------------------

	BulletEngine::BulletEngine( GameEngine& owningEngine ) : GameEngineService( owningEngine ),
															 _persistentManifoldPoolSizeInElements( 4096u ),
															 _collisionAlgorithmPoolSizeInElements( 4096u ) {}

// ---------------------------------------------------

	const UTF8Char* const BulletEngine::GetName() const {
		return UTF8L("Bullet Dynamics Engine");
	}

// ---------------------------------------------------

	void BulletEngine::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& visitor ) {
		visitor.PushSection( UTF8L("BulletDynamics") );

		visitor.Register( UTF8L("WorldPersistentManifoldPoolSizeInElements"), _persistentManifoldPoolSizeInElements );
		visitor.Register( UTF8L("WorldCollisionAlgorithmPoolSizeInElements"), _collisionAlgorithmPoolSizeInElements );
	}

// ---------------------------------------------------

	void BulletEngine::AcceptInitializationVisitor( ScriptAPIRegistrationInitializationVisitor& visitor ) {
		BulletWorldView::ExposeScriptAPI( visitor );
	}

// ---------------------------------------------------

	void BulletEngine::AcceptInitializationVisitor( WorldViewFactoryPublishingInitializationVisitor& visitor ) {
		// Reserve a little extra slack here for the aligned allocation.
		visitor.PublishFactory( this, sizeof(BulletWorldView) + (alignof(BulletWorldView) - 1u), [] ( Allocator& allocator, World& world, void* parameter ) -> ErrorCode {
			// Important to use aligned allocation. Visual Studio uses a movaps instruction for the compiler-generated btVector3::operator=() method, and it's quite common for
			// one of the vectors to end up on an unaligned boundary.
			return new(allocator, alignof(BulletWorldView), Allocator::AllocationOption::PERMANENT_ALLOCATION) BulletWorldView( world, *static_cast<BulletEngine*>(parameter) ) ? Errors::NONE : Errors::OUT_OF_MEMORY;
		} );
	}

}	// namespace Physics
}	// namespace Eldritch2