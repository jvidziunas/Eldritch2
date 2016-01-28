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

namespace {

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

// ---------------------------------------------------

	class ConeShapeX : public ::btConeShapeX {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this @ref ConeShapeX instance.
		ETInlineHint ConeShapeX( const FlatBuffers::ConeShapeX& definition ) : ::btConeShapeX( definition.Radius(), definition.Height() ) {}

		~ConeShapeX() = default;

	// ---------------------------------------------------

		ETDefineClassInstanceNew()
	};

// ---------------------------------------------------

	class ConeShapeY : public ::btConeShape {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this @ref ConeShapeZ instance.
		ETInlineHint ConeShapeY( const FlatBuffers::ConeShapeY& definition ) : ::btConeShape( definition.Radius(), definition.Height() ) {}

		~ConeShapeY() = default;

	// ---------------------------------------------------

		ETDefineClassInstanceNew()
	};

// ---------------------------------------------------

	class ConeShapeZ : public ::btConeShapeZ {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this @ref ConeShapeZ instance.
		ETInlineHint ConeShapeZ( const FlatBuffers::ConeShapeZ& definition ) : ::btConeShapeZ( definition.Radius(), definition.Height() ) {}

		~ConeShapeZ() = default;

	// ---------------------------------------------------

		ETDefineClassInstanceNew()
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	ArticulatedBodyResourceView::ArticulatedBodyResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ),
																																										   _shapeAllocator( allocator, UTF8L("Bullet Articulated Body Collision Shape Allocator") ),
																																										   _shapes( { allocator, UTF8L("Bullet Articulated Body View Subshape Collection Allocator") } ) {}

// ---------------------------------------------------

	ArticulatedBodyResourceView::~ArticulatedBodyResourceView() {
		using ShapeCollection	= decltype(_shapes);

	// ---

		auto&	allocator( _shapeAllocator );

		_shapes.ClearAndDispose( [&allocator] ( const ShapeCollection::ValueType& shape ) {
			allocator.Delete( shape );
		} );
	}

// ---------------------------------------------------

	ErrorCode ArticulatedBodyResourceView::UpdateFromByteStream( const Range<const char*> bytes ) {
		const auto	bodyDefinition( ::Eldritch2::Physics::FlatBuffers::GetArticulatedBody( bytes.first ) );

		if( !bodyDefinition || !bodyDefinition->Bodies() ) {
			return Error::InvalidParameter;
		}

		_shapes.Reserve( bodyDefinition->Bodies()->size() );

		for( auto shapeDefinition( bodyDefinition->Bodies()->begin() ), end( bodyDefinition->Bodies()->end() ); shapeDefinition != end; ++shapeDefinition ) {
			switch( shapeDefinition->Shape_type() ) {
				case FlatBuffers::CollisionShape::SphereShape: {
					const auto&	definition( *static_cast<const FlatBuffers::SphereShape*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(SphereShape), Allocator::AllocationDuration::Normal) SphereShape( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::BoxShape: {
					const auto&	definition( *static_cast<const FlatBuffers::BoxShape*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(BoxShape), Allocator::AllocationDuration::Normal) BoxShape( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::CapsuleShapeX: {
					const auto&	definition( *static_cast<const FlatBuffers::CapsuleShapeX*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(CapsuleShapeX), Allocator::AllocationDuration::Normal) CapsuleShapeX( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::CapsuleShapeY: {
					const auto&	definition( *static_cast<const FlatBuffers::CapsuleShapeY*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(CapsuleShapeY), Allocator::AllocationDuration::Normal) CapsuleShapeY( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::CapsuleShapeZ: {
					const auto&	definition( *static_cast<const FlatBuffers::CapsuleShapeZ*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(CapsuleShapeZ), Allocator::AllocationDuration::Normal) CapsuleShapeZ( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::ConeShapeX: {
					const auto&	definition( *static_cast<const FlatBuffers::ConeShapeX*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(ConeShapeX), Allocator::AllocationDuration::Normal) ConeShapeX( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::ConeShapeY: {
					const auto&	definition( *static_cast<const FlatBuffers::ConeShapeY*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(ConeShapeY), Allocator::AllocationDuration::Normal) ConeShapeY( definition ) );
					break;
				}
				case FlatBuffers::CollisionShape::ConeShapeZ: {
					const auto&	definition( *static_cast<const FlatBuffers::ConeShapeZ*>(shapeDefinition->Shape()) );

					_shapes.PushBack( new(_shapeAllocator, alignof(ConeShapeZ), Allocator::AllocationDuration::Normal) ConeShapeZ( definition ) );
					break;
				}
			};
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