/*==================================================================*\
  MeshResourceView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/MeshResourceView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Physics/ArticulatedBody_generated.h>
//------------------------------------------------------------------//
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
#include <PxArticulationLink.h>
#include <PxArticulation.h>
#include <PxPhysics.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Physics;
using namespace ::Eldritch2;

using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	MeshResourceView::RigidBody::RigidBody( const uint16 bodyIndex, UniquePointer<PxShape>&& shape ) : _shape( ::std::move( shape ) ), _bodyIndex( bodyIndex ) {
		_shape->userData = this;
	}

// ---------------------------------------------------

	MeshResourceView::RigidBody::RigidBody( RigidBody&& body ) : _shape( ::std::move( body._shape ) ), _bodyIndex( body._bodyIndex ) {}

// ---------------------------------------------------

	MeshResourceView::MeshResourceView( ContentLibrary& owningLibrary, ContentPackage& package, const UTF8Char* const name, Allocator& allocator ) : ResourceView( owningLibrary, package, name, allocator ),
																																					 _rigidBodies( { allocator, UTF8L("PhysX Mesh Resource View Rigid Body Definition Allocator") } ),
																																					 _softBodies( { allocator, UTF8L("PhysX Mesh Resource View Soft Body Definition Allocator") } ) {}

// ---------------------------------------------------

	ErrorCode MeshResourceView::UpdateFromByteStream( const Range<const char*> bytes ) {
		class CapsuleGeometry : public PxCapsuleGeometry {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref CapsuleGeometry instance.
			/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref PxCapsuleGeometry members with.
				*/
			ETForceInlineHint CapsuleGeometry( const FlatBuffers::CapsuleShapeX& definition ) : PxCapsuleGeometry( definition.Radius(), (definition.Height() / 2.0f) ) {}
			//!	Constructs this @ref CapsuleGeometry instance.
			/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref CapsuleGeometry members with.
				*/
			ETForceInlineHint CapsuleGeometry( const FlatBuffers::CapsuleShapeY& definition ) : PxCapsuleGeometry( definition.Radius(), (definition.Height() / 2.0f) ) {}
			//!	Constructs this @ref CapsuleGeometry instance.
			/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref CapsuleGeometry members with.
				*/
			ETForceInlineHint CapsuleGeometry( const FlatBuffers::CapsuleShapeZ& definition ) : PxCapsuleGeometry( definition.Radius(), (definition.Height() / 2.0f) ) {}

			~CapsuleGeometry() = default;
		};

		class SphereGeometry : public PxSphereGeometry {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref SphereGeometry instance.
			/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref PxSphereGeometry members with.
				*/
			ETForceInlineHint SphereGeometry( const FlatBuffers::SphereShape& definition ) : PxSphereGeometry( definition.Radius() ) {}

			~SphereGeometry() = default;
		};

		class BoxGeometry : public PxBoxGeometry {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			ETForceInlineHint BoxGeometry( const FlatBuffers::BoxShape& definition ) : PxBoxGeometry( definition.HalfExtent()->X(), definition.HalfExtent()->Y(), definition.HalfExtent()->Z() ) {}

			~BoxGeometry() = default;
		};

	// ---

		auto	asset( FlatBuffers::GetArticulatedBody( bytes.first ) );

		_rigidBodies.Clear();

		if( !asset || !asset->RigidBodies() ) {
			return Error::InvalidParameter;
		}

		_rigidBodies.SetCapacity( asset->RigidBodies()->size() );

		for( auto definition( asset->RigidBodies()->begin() ), end( asset->RigidBodies()->end() ); definition != end; ++definition ) {
			UniquePointer<PxMaterial>	newMaterial( PxGetPhysics().createMaterial( definition->Friction(), definition->RollingFriction(), definition->Restitution() ) );
			UniquePointer<PxShape>		shape;

			if( !newMaterial ) {
				return Error::OutOfMemory;
			}

			switch( definition->Shape_type() ) {
				case FlatBuffers::CollisionShape::SphereShape: {
					shape.reset( PxGetPhysics().createShape( SphereGeometry( *static_cast<const FlatBuffers::SphereShape*>(definition->Shape()) ), *newMaterial ) );
					break;
				}	// case FlatBuffers::CollisionShape::SphereShape

				case FlatBuffers::CollisionShape::BoxShape: {
					shape.reset( PxGetPhysics().createShape( BoxGeometry( *static_cast<const FlatBuffers::BoxShape*>(definition->Shape()) ), *newMaterial ) );
					break;
				}	// case FlatBuffers::CollisionShape::BoxShape

				case FlatBuffers::CollisionShape::CapsuleShapeX: {
					shape.reset( PxGetPhysics().createShape( CapsuleGeometry( *static_cast<const FlatBuffers::CapsuleShapeX*>(definition->Shape()) ), *newMaterial ) );
					break;
				}	// case FlatBuffers::CollisionShape::CapsuleShapeX

				case FlatBuffers::CollisionShape::CapsuleShapeY: {
					shape.reset( PxGetPhysics().createShape( CapsuleGeometry( *static_cast<const FlatBuffers::CapsuleShapeY*>(definition->Shape()) ), *newMaterial ) );
					// PhysX does not have a separate class for different orientations of capsule. Instead, we use a local shape transformation to mimic the same behavior.
					if( shape ) {
						shape->setLocalPose( { 0.0f, 0.0f, 0.0f, { PxHalfPi, { 0.0f, 0.0f, 1.0f } } } );
					}
					
					break;
				}	// case FlatBuffers::CollisionShape::CapsuleShapeY

				case FlatBuffers::CollisionShape::CapsuleShapeZ: {
					shape.reset( PxGetPhysics().createShape( CapsuleGeometry( *static_cast<const FlatBuffers::CapsuleShapeZ*>(definition->Shape()) ), *newMaterial ) );
					// PhysX does not have a separate class for different orientations of capsule. Instead, we use a local shape transformation to mimic the same behavior.
					if( shape ) {
						shape->setLocalPose( { 0.0f, 0.0f, 0.0f, { PxHalfPi, { 0.0f, 1.0f, 0.0f } } } );
					}
					break;
				}	// case FlatBuffers::CollisionShape::CapsuleShapeZ

				default: {
					return Error::InvalidParameter;
				}
			};

			if( !shape ) {
				return Error::OutOfMemory;
			}

			_rigidBodies.EmplaceBack( definition->BoneIndex(), ::std::move( shape ) );
		}

		return Error::None;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const MeshResourceView::GetSerializedDataTag() {
		return FlatBuffers::ArticulatedBodyExtension();
	}

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2