/*==================================================================*\
  MeshResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/MeshResourceView.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/Range.hpp>
#include <Packages/ContentPackage.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Physics/ArticulatedBody_generated.h>
//------------------------------------------------------------------//
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btConeShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	MeshResourceView::RigidBody::RigidBody( const FlatBuffers::RigidBody& definition, Allocator& allocator ) : boneIndex( definition.BoneIndex() ),
																											   shape( nullptr ),
																											   mass( definition.Mass() ),
																											   linearDamping( definition.LinearDamping() ),
																											   angularDamping( definition.AngularDamping() ),
																											   friction( definition.Friction() ),
																											   rollingFriction( definition.RollingFriction() ),
																											   restitution( definition.Restitution() ) {
		class SphereShape : public ::btSphereShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this @ref SphereShape instance.
			ETInlineHint SphereShape( const FlatBuffers::SphereShape& definition ) : ::btSphereShape( definition.Radius() ) {}

			~SphereShape() = default;

		// ---------------------------------------------------

			ETDefineClassInstanceNew()
		};

	// ---------------------------------------------------

		class BoxShape : public ::btBoxShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this @ref BoxShape instance.
			ETInlineHint BoxShape( const FlatBuffers::BoxShape& definition ) : ::btBoxShape( ::btVector3( definition.HalfExtent()->X(), definition.HalfExtent()->Y(), definition.HalfExtent()->Z() ) ) {}

			~BoxShape() = default;

		// ---------------------------------------------------

			ETDefineClassInstanceNew()
		};

	// ---------------------------------------------------

		class CapsuleShapeX : public ::btCapsuleShapeX {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this @ref CapsuleShapeX instance.
			ETInlineHint CapsuleShapeX( const FlatBuffers::CapsuleShapeX& definition ) : ::btCapsuleShapeX( definition.Radius(), definition.Height() ) {}

			~CapsuleShapeX() = default;

		// ---------------------------------------------------

			ETDefineClassInstanceNew()
		};

	// ---------------------------------------------------

		class CapsuleShapeY : public ::btCapsuleShape {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this @ref CapsuleShape instance.
			ETInlineHint CapsuleShapeY( const FlatBuffers::CapsuleShapeY& definition ) : ::btCapsuleShape( definition.Radius(), definition.Height() ) {}

			~CapsuleShapeY() = default;

		// ---------------------------------------------------

			ETDefineClassInstanceNew()
		};

	// ---------------------------------------------------

		class CapsuleShapeZ : public ::btCapsuleShapeZ {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this @ref CapsuleShapeZ instance.
			ETInlineHint CapsuleShapeZ( const FlatBuffers::CapsuleShapeZ& definition ) : ::btCapsuleShapeZ( definition.Radius(), definition.Height() ) {}

			~CapsuleShapeZ() = default;

		// ---------------------------------------------------

			ETDefineClassInstanceNew()
		};

	// ---

		switch( definition.Shape_type() ) {
			case FlatBuffers::CollisionShape::SphereShape: {
				shape = new(allocator, alignof( SphereShape ), Allocator::AllocationDuration::Normal) SphereShape( *static_cast<const FlatBuffers::SphereShape*>(definition.Shape()) );
				break;
			}
			case FlatBuffers::CollisionShape::BoxShape: {
				shape = new(allocator, alignof( BoxShape ), Allocator::AllocationDuration::Normal) BoxShape( *static_cast<const FlatBuffers::BoxShape*>(definition.Shape()) );
				break;
			}
			case FlatBuffers::CollisionShape::CapsuleShapeX: {
				shape = new(allocator, alignof( CapsuleShapeX ), Allocator::AllocationDuration::Normal) CapsuleShapeX( *static_cast<const FlatBuffers::CapsuleShapeX*>(definition.Shape()) );
				break;
			}
			case FlatBuffers::CollisionShape::CapsuleShapeY: {
				shape = new(allocator, alignof( CapsuleShapeY ), Allocator::AllocationDuration::Normal) CapsuleShapeY( *static_cast<const FlatBuffers::CapsuleShapeY*>(definition.Shape()) );
				break;
			}
			case FlatBuffers::CollisionShape::CapsuleShapeZ: {
				shape = new(allocator, alignof( CapsuleShapeZ ), Allocator::AllocationDuration::Normal) CapsuleShapeZ( *static_cast<const FlatBuffers::CapsuleShapeZ*>(definition.Shape()) );
				break;
			}
		};
	}

// ---------------------------------------------------

	MeshResourceView::MeshResourceView( const UTF8Char* const name, Allocator& allocator ) : ResourceView( name ),
																							 _shapeAllocator( allocator, UTF8L("Bullet Mesh View Rigid Body Collision Shape Allocator") ),
																							 _rigidBodies( { allocator, UTF8L("Bullet Mesh View Rigid Body Collection Allocator") } ) {}

// ---------------------------------------------------

	MeshResourceView::~MeshResourceView() {
		auto&	allocator( _shapeAllocator );

		_rigidBodies.ClearAndDispose( [&allocator] ( const RigidBody& body ) {
			allocator.Delete( body.shape, ::Eldritch2::AlignedDeallocationSemantics );
		} );
	}

// ---------------------------------------------------

	const ::Eldritch2::ResizableArray<MeshResourceView::RigidBody>& MeshResourceView::GetBodies() const {
		return _rigidBodies;
	}

// ---------------------------------------------------

	ErrorCode MeshResourceView::AttachToPackage( const Range<const char*> bytes, ContentPackage& /*package*/, ContentLibrary& library ) {
		const auto	asset( FlatBuffers::GetArticulatedBody( bytes.first ) );

		_rigidBodies.Clear();

		if( !asset || !asset->RigidBodies() ) {
			return Error::InvalidParameter;
		}

		_rigidBodies.SetCapacity( asset->RigidBodies()->size() );

		for( auto definition( asset->RigidBodies()->begin() ), end( asset->RigidBodies()->end() ); definition != end; ++definition ) {
			_rigidBodies.EmplaceBack( **definition, _shapeAllocator );
		}

		PublishToLibraryAs<decltype(*this)>( library );

		return Error::None;
	}

// ---------------------------------------------------

	void MeshResourceView::DetachFromPackage( ContentPackage& /*package*/, ContentLibrary& library ) const {
		RemoveFromLibraryAs<decltype(*this)>( library );
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return FlatBuffers::ArticulatedBodyExtension();
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2