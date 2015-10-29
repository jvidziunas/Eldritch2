/*==================================================================*\
  CollisionShapeResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/CollisionShapeResourceView.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	CollisionShapeResourceView::CollisionShapeResourceView( const UTF8Char* const name, Allocator& allocator ) : ResourceView( name, allocator ),
																												 _shapeAllocator( allocator, UTF8L("Bullet Collision Shape Allocator") ),
																												 _shapes( { allocator, UTF8L("Bullet Collision Shape View Subshape Collection Allocator") } ) {}

// ---------------------------------------------------

	CollisionShapeResourceView::~CollisionShapeResourceView() {
		using ShapeCollection	= decltype(_shapes);

	// ---

		auto&	allocator( _shapeAllocator );

		_shapes.ClearAndDispose( [&allocator] ( const ShapeCollection::ValueType& shape ) {
			allocator.Delete( *shape.second );
		} );
	}

// ---------------------------------------------------

	ErrorCode CollisionShapeResourceView::UpdateFromByteStream( const Range<const char*> /*bytes*/ ) {
		return Error::NONE;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const CollisionShapeResourceView::GetSerializedDataTag() {
		return UTF8L("CollisionShape");
	}

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2