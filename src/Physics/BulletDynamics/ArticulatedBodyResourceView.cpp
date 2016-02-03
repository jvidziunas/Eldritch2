/*==================================================================*\
  ArticulatedBodyResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/ArticulatedBodyResourceView.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Physics/ArticulatedBody_generated.h>
//------------------------------------------------------------------//
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

	ArticulatedBodyResourceView::Body::Body( const FlatBuffers::Body& definition, Allocator& allocator ) : boneIndex( definition.BoneIndex() ),
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

	ArticulatedBodyResourceView::ArticulatedBodyResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ),
																																										   _shapeAllocator( allocator, UTF8L("Bullet Articulated Body Collision Shape Allocator") ),
																																										   _bodies( { allocator, UTF8L("Bullet Articulated Body View Subshape Collection Allocator") } ) {}

// ---------------------------------------------------

	ArticulatedBodyResourceView::~ArticulatedBodyResourceView() {
		auto&	allocator( _shapeAllocator );

		_bodies.ClearAndDispose( [&allocator] ( const Body& body ) {
			allocator.Delete( body.shape, ::Eldritch2::AlignedDeallocationSemantics );
		} );
	}

// ---------------------------------------------------

	const ::Eldritch2::ResizableArray<ArticulatedBodyResourceView::Body>& ArticulatedBodyResourceView::GetBodies() const {
		return _bodies;
	}

// ---------------------------------------------------

	ErrorCode ArticulatedBodyResourceView::UpdateFromByteStream( const Range<const char*> bytes ) {
		const auto	asset( ::Eldritch2::Physics::FlatBuffers::GetArticulatedBody( bytes.first ) );

		_bodies.Clear();

		if( !asset || !asset->Bodies() ) {
			return Error::InvalidParameter;
		}

		_bodies.Reserve( asset->Bodies()->size() );

		for( auto definition( asset->Bodies()->begin() ), end( asset->Bodies()->end() ); definition != end; ++definition ) {
			_bodies.EmplaceBack( **definition, _shapeAllocator );
		}

		return Error::None;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ArticulatedBodyResourceView::GetSerializedDataTag() {
		return FlatBuffers::ArticulatedBodyExtension();
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2