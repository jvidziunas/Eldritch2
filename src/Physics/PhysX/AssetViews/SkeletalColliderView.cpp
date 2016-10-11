/*==================================================================*\
  SkeletalColliderView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/SkeletalColliderView.hpp>
#include <Utility/Containers/Range.hpp>
#include <Assets/AssetLibrary.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/SkeletalCollisionAsset_generated.h>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
#	pragma warning( disable : 6326 )
#endif
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
#include <PxArticulationLink.h>
#include <PxArticulation.h>
#include <PxAggregate.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )
#endif
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {
namespace {

	class CapsuleGeometry : public PxCapsuleGeometry {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref CapsuleGeometry instance.
	/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref PxCapsuleGeometry members with. */
		ETForceInlineHint CapsuleGeometry( const FlatBuffers::CapsuleShapeX& definition ) : PxCapsuleGeometry( definition.Radius(), (definition.Height() / 2.0f) ) {}
	//!	Constructs this @ref CapsuleGeometry instance.
	/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref CapsuleGeometry members with. */
		ETForceInlineHint CapsuleGeometry( const FlatBuffers::CapsuleShapeY& definition ) : PxCapsuleGeometry( definition.Radius(), (definition.Height() / 2.0f) ) {}
	//!	Constructs this @ref CapsuleGeometry instance.
	/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref CapsuleGeometry members with. */
		ETForceInlineHint CapsuleGeometry( const FlatBuffers::CapsuleShapeZ& definition ) : PxCapsuleGeometry( definition.Radius(), (definition.Height() / 2.0f) ) {}

		~CapsuleGeometry() = default;
	};

// ---

	class SphereGeometry : public PxSphereGeometry {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SphereGeometry instance.
	/*!	@param[in] definition FlatBuffer struct containing the fields to initialize the @ref PxSphereGeometry members with. */
		ETForceInlineHint SphereGeometry( const FlatBuffers::SphereShape& definition ) : PxSphereGeometry( definition.Radius() ) {}

		~SphereGeometry() = default;
	};

// ---

	class BoxGeometry : public PxBoxGeometry {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref BoxGeometry instance.
		ETForceInlineHint BoxGeometry( const FlatBuffers::BoxShape& definition ) : PxBoxGeometry( definition.HalfExtent()->X(), definition.HalfExtent()->Y(), definition.HalfExtent()->Z() ) {}

		~BoxGeometry() = default;
	};

// ---

	static ETInlineHint ETPureFunctionHint Result<Eldritch2::UniquePointer<SkeletalColliderView::RigidShape[]>> CreateRigidShapes( Allocator& allocator, PxPhysics& physics, const Vector<Offset<FlatBuffers::RigidBodyDescriptor>>& bodies ) {
		Eldritch2::UniquePointer<SkeletalColliderView::RigidShape[]>	rigidShapes( static_cast<SkeletalColliderView::RigidShape*>( allocator.Allocate( bodies.size() * sizeof(SkeletalColliderView::RigidShape), AllocationDuration::Normal ) ), { bodies.size(), allocator } );

		SkeletalColliderView::RigidShape*								out( rigidShapes.Begin() );
		for( const auto definition : bodies ) {
			UniquePointer<PxMaterial>	physxMaterial( physics.createMaterial( definition->Friction(), definition->RollingFriction(), definition->Restitution() ) );
			if( !physxMaterial ) {
				return Error::Unspecified;
			}

			UniquePointer<PxShape>	physxShape;

			switch( definition->Shape_type() ) {
				case FlatBuffers::CollisionShape::SphereShape: {
					physxShape.Reset( physics.createShape( SphereGeometry( *static_cast<const FlatBuffers::SphereShape*>( definition->Shape() ) ), *physxMaterial ) );
					break;
				}	// case FlatBuffers::CollisionShape::SphereShape

				case FlatBuffers::CollisionShape::BoxShape: {
					physxShape.Reset( physics.createShape( BoxGeometry( *static_cast<const FlatBuffers::BoxShape*>( definition->Shape() ) ), *physxMaterial ) );
					break;
				}	// case FlatBuffers::CollisionShape::BoxShape

				case FlatBuffers::CollisionShape::CapsuleShapeX: {
					physxShape.Reset( physics.createShape( CapsuleGeometry( *static_cast<const FlatBuffers::CapsuleShapeX*>( definition->Shape() ) ), *physxMaterial ) );
					break;
				}	// case FlatBuffers::CollisionShape::CapsuleShapeX

				case FlatBuffers::CollisionShape::CapsuleShapeY: {
					physxShape.Reset( physics.createShape( CapsuleGeometry( *static_cast<const FlatBuffers::CapsuleShapeY*>( definition->Shape() ) ), *physxMaterial ) );
				//	PhysX does not have a separate class for different orientations of capsule. Instead, we use a local physxShape transformation to mimic the same behavior.
					if( physxShape ) {
						physxShape->setLocalPose( { 0.0f, 0.0f, 0.0f, { PxHalfPi, { 0.0f, 0.0f, 1.0f } } } );
					}

					break;
				}	// case FlatBuffers::CollisionShape::CapsuleShapeY

				case FlatBuffers::CollisionShape::CapsuleShapeZ: {
					physxShape.Reset( physics.createShape( CapsuleGeometry( *static_cast<const FlatBuffers::CapsuleShapeZ*>( definition->Shape() ) ), *physxMaterial ) );
				//	PhysX does not have a separate class for different orientations of capsule. Instead, we use a local physxShape transformation to mimic the same behavior.
					if( physxShape ) {
						physxShape->setLocalPose( { 0.0f, 0.0f, 0.0f, { PxHalfPi, { 0.0f, 1.0f, 0.0f } } } );
					}
					break;
				}	// case FlatBuffers::CollisionShape::CapsuleShapeZ

				default: {
					return nullptr;
				}
			};

			new( out++ ) SkeletalColliderView::RigidShape( eastl::move( physxShape ), definition->BoneIndex() );
		}

		return eastl::move( rigidShapes );
	}

// ---

	static ETInlineHint ETPureFunctionHint Result<Eldritch2::UniquePointer<SkeletalColliderView::ClothShape[]>> CreateClothShapes( Allocator& allocator, PxPhysics& physics, const Vector<Offset<FlatBuffers::ClothBodyDescriptor>>& bodies ) {
		return MakeUniqueArray<SkeletalColliderView::ClothShape>( allocator, bodies.size() );
	}

}	// anonymous namespace

	SkeletalColliderView::RigidShape::RigidShape( UniquePointer<PxShape> shape, BoneIndex localBoneIndex ) : physicsShape( eastl::move( shape ) ), localBoneIndex( localBoneIndex ) {}

// ---------------------------------------------------

	SkeletalColliderView::SkeletalColliderView( const Utf8Char* const name, Eldritch2::UniquePointer<RigidShape[]> rigidShapes, Eldritch2::UniquePointer<ClothShape[]> clothShapes ) : AssetView( name ),
																																													   _rigidShapes( eastl::move( rigidShapes ) ),
																																													   _clothShapes( eastl::move( clothShapes ) ) {}

// ---------------------------------------------------

	Result<Eldritch2::UniquePointer<AssetView>>	SkeletalColliderView::CreateView( Allocator& allocator, PxPhysics& physics, const AssetLibrary& library, const Utf8Char* const name, Range<const char*> rawBytes ) {
		Verifier	verifier( reinterpret_cast<const uint8_t*>( rawBytes.Begin() ), rawBytes.GetSize() );

		if( !FlatBuffers::SkeletalCollisionAssetBufferHasIdentifier( rawBytes.Begin() ) || !FlatBuffers::VerifySkeletalCollisionAssetBuffer( verifier ) ) {
			library.GetLog()( MessageSeverity::Error, "Skeletal collider asset '{}' is malformed!"  ET_UTF8_NEWLINE_LITERAL, name );
			return Error::InvalidParameter;
		}

		const FlatBuffers::SkeletalCollisionAsset&	asset( *FlatBuffers::GetSkeletalCollisionAsset( rawBytes.Begin() ) );
	
		Result<Eldritch2::UniquePointer<SkeletalColliderView::RigidShape[]>>	createRigidShapesResult( CreateRigidShapes( allocator, physics, *asset.RigidBodies() ) );
		if( !createRigidShapesResult ) {
			library.GetLog()( MessageSeverity::Error, "Rigid shape data for skeletal collider asset '{}' is malformed!"  ET_UTF8_NEWLINE_LITERAL, name );
			return createRigidShapesResult.GetErrorCode();
		}

		Result<Eldritch2::UniquePointer<SkeletalColliderView::ClothShape[]>>	createClothShapesResult( CreateClothShapes( allocator, physics, *asset.ClothBodies() ) );
		if( !createClothShapesResult ) {
			library.GetLog()( MessageSeverity::Error, "Cloth data for skeletal collider asset '{}' is malformed!"  ET_UTF8_NEWLINE_LITERAL, name );
			return createClothShapesResult.GetErrorCode();
		}

		auto resultView( MakeUnique<SkeletalColliderView>( allocator, name, eastl::move( *createRigidShapesResult ), eastl::move( *createClothShapesResult ) ) );
		if( !resultView ) {
			return Error::OutOfMemory;
		}

		return eastl::move( resultView );
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal SkeletalColliderView::GetExtension() {
		return Utf8Literal( FlatBuffers::SkeletalCollisionAssetExtension() );
	}

// ---------------------------------------------------

	void SkeletalColliderView::AttachTo( PxArticulation& articulation ) const {
		const auto	links( static_cast<PxArticulationLink**>( _alloca( sizeof(PxArticulationLink*) * articulation.getNbLinks() ) ) );

		articulation.getLinks( links, articulation.getNbLinks() );

		for( const auto& shape : _rigidShapes ) {
			links[shape.localBoneIndex]->attachShape( *shape.physicsShape );
		}
	}

// ---------------------------------------------------

	void SkeletalColliderView::AttachTo( PxAggregate& aggregate ) const {
		const auto	actors( static_cast<PxActor**>( _alloca( sizeof(PxActor*) * aggregate.getNbActors() ) ) );

		aggregate.getActors( actors, aggregate.getNbActors() );

		for( const auto& shape : _rigidShapes ) {
			actors[shape.localBoneIndex]->is<PxRigidActor>()->attachShape( *shape.physicsShape );
		}
	}

// ---------------------------------------------------

	void SkeletalColliderView::DetachFrom( PxArticulation& articulation ) const {
		const auto	links( static_cast<PxArticulationLink**>( _alloca( sizeof(PxArticulationLink*) * articulation.getNbLinks() ) ) );

		articulation.getLinks( links, articulation.getNbLinks() );

		for( const auto& shape : _rigidShapes ) {
			links[shape.localBoneIndex]->detachShape( *shape.physicsShape );
		}
	}

// ---------------------------------------------------

	void SkeletalColliderView::DetachFrom( PxAggregate& aggregate ) const {
		const auto	actors( static_cast<PxActor**>( _alloca( sizeof(PxActor*) * aggregate.getNbActors() ) ) );

		aggregate.getActors( actors, aggregate.getNbActors() );

		for( const auto& shape : _rigidShapes ) {
			actors[shape.localBoneIndex]->is<PxRigidActor>()->detachShape( *shape.physicsShape );
		}
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2