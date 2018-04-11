/*==================================================================*\
  PhysicsAsset.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/PhysicsBody_generated.h>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <geometry/PxCapsuleGeometry.h>
#	include <geometry/PxSphereGeometry.h>
#	include <geometry/PxBoxGeometry.h>
#	include <PxArticulationLink.h>
#	include <PxArticulation.h>
#	include <PxMaterial.h>
#	include <PxPhysics.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace AssetViews {

	using namespace ::Eldritch2::Physics::FlatBuffers;
	using namespace ::Eldritch2::FlatBuffers;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;
	using namespace ::physx;

namespace {

	ETInlineHint ETPureFunctionHint PxVec3 AsPxVec3( const Float3* vector ) {
		return PxVec3{ vector->X(), vector->Y(), vector->Z() };
	}

// ---------------------------------------------------

	ETInlineHint PhysicsAsset::RigidShape ParseShape( PxPhysics& physx, BoneIndex boneIndex, PxMaterial& material, const SphereShape& shape ) {
		return { physx.createShape( PxSphereGeometry( shape.Radius() ), material ), boneIndex };
	}

// ---------------------------------------------------

	ETInlineHint PhysicsAsset::RigidShape ParseShape( PxPhysics& physx, BoneIndex boneIndex, PxMaterial& material, const BoxShape& shape ) {
		return { physx.createShape( PxBoxGeometry( AsPxVec3( shape.HalfExtent() ) ), material ), boneIndex };
	}

// ---------------------------------------------------

	ETInlineHint PhysicsAsset::RigidShape ParseShape( PxPhysics& physx, BoneIndex boneIndex, PxMaterial& material, const CapsuleShape& shape ) {
		return { physx.createShape( PxCapsuleGeometry( shape.Radius(), shape.Height() * 0.5f ), material ), boneIndex };
	}

// ---------------------------------------------------

	ETInlineHint Result<ArrayList<PhysicsAsset::RigidShape>> CreateRigidShapes( const MallocAllocator& allocator, PxPhysics& physx, const flatbuffers::Vector<Offset<RigidBody>>& bodies ) {
		ArrayList<PhysicsAsset::RigidShape>	shapes( bodies.size(), allocator );

		for (const RigidBody* body : bodies) {
			PhysxPointer<PxMaterial>	material( physx.createMaterial( body->Friction(), body->RollingFriction(), body->Restitution() ) );
			const BoneIndex				boneIndex( body->BoneIndex() );

			if (!material) {
				return Error::Unspecified;
			}

			switch (body->Shape_type()) {
				case CollisionShape::NONE:			{ break; }
				case CollisionShape::SphereShape:	{ shapes.Append( ParseShape( physx, boneIndex, *material, *static_cast<const SphereShape*>(body->Shape()) ) ); break; }
				case CollisionShape::BoxShape:		{ shapes.Append( ParseShape( physx, boneIndex, *material, *static_cast<const BoxShape*>(body->Shape()) ) ); break; }
				case CollisionShape::CapsuleShape:	{ shapes.Append( ParseShape( physx, boneIndex, *material, *static_cast<const CapsuleShape*>(body->Shape()) ) ); break; }
			//	Note that since we're reading from a disk asset, we *cannot* assume that the input value will always correspond to a known enumeration value.
				default:							{ return Error::InvalidParameter; }
			};
		}

	//	Transfer ownership to the caller.
		return eastl::move( shapes );
	}

// ---------------------------------------------------

	static ETInlineHint Result<ArrayList<PhysicsAsset::ClothShape>> CreateClothShapes( const MallocAllocator& allocator, PxPhysics& /*physx*/, const flatbuffers::Vector<Offset<ClothBody>>& bodies ) {
		ArrayList<PhysicsAsset::ClothShape>	shapes( bodies.size(), allocator );

		for (const ClothBody* body : bodies) {
			shapes.EmplaceBack();
		}

	//	Transfer ownership to the caller.
		return eastl::move( shapes );
	}

}	// anonymous namespace

	PhysicsAsset::RigidShape::RigidShape( PhysxPointer<PxShape> shape, BoneIndex linkIndex ) : shape( eastl::move( shape ) ), linkIndex( linkIndex ) {}

// ---------------------------------------------------

	PhysicsAsset::PhysicsAsset(
		const Utf8Char* const filePath
	) : Asset( filePath ),
		_rigidShapes( MallocAllocator( "PhysX Physics Asset Rigid Shape Collection Allocator" ) ),
		_clothShapes( MallocAllocator( "PhysX Physics Asset Cloth Shape Collection Allocator" ) ) {
	}

// ---------------------------------------------------

	const ArrayList<PhysicsAsset::RigidShape>& PhysicsAsset::GetRigidShapes() const {
		return _rigidShapes;
	}

// ---------------------------------------------------

	const ArrayList<PhysicsAsset::ClothShape>& PhysicsAsset::GetClothShapes() const {
		return _clothShapes;
	}

// ---------------------------------------------------

	ErrorCode PhysicsAsset::BindResources( const Builder& asset ) {
	//	Verify the data we are considering can plausibly represent a collision asset.
		Verifier verifier( reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize() );
		if (!VerifyPhysicsBodyBuffer( verifier )) {
			asset.WriteLog( MessageType::Error, "{} is malformed!" UTF8_NEWLINE, GetPath() );
			return Error::InvalidParameter;
		}

		const PhysicsBody*	physicsAsset( GetPhysicsBody( asset.Begin() ) );

		Result<ArrayList<RigidShape>> createRigidShapesResult( CreateRigidShapes( _rigidShapes.GetAllocator(), PxGetPhysics(), *physicsAsset->RigidShapes() ) );
		if (Failed( createRigidShapesResult )) {
			asset.WriteLog( MessageType::Error, "Rigid shape data for {} is malformed!" UTF8_NEWLINE, GetPath() );
			return createRigidShapesResult;
		}

		Result<ArrayList<ClothShape>> createClothShapesResult( CreateClothShapes( _clothShapes.GetAllocator(), PxGetPhysics(), *physicsAsset->ClothShapes() ) );
		if (Failed( createClothShapesResult )) {
			asset.WriteLog( MessageType::Error, "Cloth shape data for {} is malformed!" UTF8_NEWLINE, GetPath() );
			return createClothShapesResult;
		}

		Swap( _rigidShapes, *createRigidShapesResult );
		Swap( _clothShapes, *createClothShapesResult );

		return Error::None;
	}

// ---------------------------------------------------

	void PhysicsAsset::FreeResources() {
		_rigidShapes.Clear( ReleaseMemorySemantics() );
		_clothShapes.Clear( ReleaseMemorySemantics() );
	}

}	// namespace AssetViews
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2