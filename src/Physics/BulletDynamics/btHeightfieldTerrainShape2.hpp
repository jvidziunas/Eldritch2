/*==================================================================*\
  btHeightfieldTerrainShape2.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
//------------------------------------------------------------------//

ATTRIBUTE_ALIGNED16(class) btHeightfieldTerrainShape2 : public ::btHeightfieldTerrainShape {
public:
	BT_DECLARE_ALIGNED_ALLOCATOR();

// ---------------------------------------------------

	/// preferred constructor
	/**
	This constructor supports a range of heightfield
	data types, and allows for a non-zero minimum height value.
	heightScale is needed for any integer-based heightfield data types.
	*/
	btHeightfieldTerrainShape2( int heightStickWidth, int heightStickLength,
								 const void* heightfieldData, btScalar heightScale,
								 btScalar minHeight, btScalar maxHeight,
								 int upAxis, PHY_ScalarType heightDataType,
								 bool flipQuadEdges );

	virtual ~btHeightfieldTerrainShape2() = default;

// ---------------------------------------------------

	virtual void	processAllTriangles( btTriangleCallback* callback, const btVector3& aabbMin, const btVector3& aabbMax ) const;
};