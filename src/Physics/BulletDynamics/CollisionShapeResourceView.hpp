/*==================================================================*\
  CollisionShapeResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/FlatOrderedMap.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

class	btCollisionShape;

namespace Eldritch2 {
namespace Physics {
namespace BulletDynamics {

	class CollisionShapeResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref CollisionShapeResourceView instance.
		CollisionShapeResourceView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref CollisionShapeResourceView instance.
		~CollisionShapeResourceView();

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	InstantiateFromByteArray( const ::Eldritch2::Range<const char*>& sourceBytes );

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator												_shapeAllocator;
		::Eldritch2::FlatOrderedMap<::Eldritch2::uint32, ::btCollisionShape*>	_shapes;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2