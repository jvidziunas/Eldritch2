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
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
#include <PxArticulationLink.h>
#include <PxArticulation.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	using namespace ::Eldritch2::Physics::FlatBuffers;
	using namespace ::Eldritch2::FlatBuffers;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;
	using namespace ::physx;

	namespace {

		ETInlineHint ETPureFunctionHint PxVec3 AsPxVec3(const Float3* vector) {
			return PxVec3 { vector->X(), vector->Y(), vector->Z() };
		}

		// ---------------------------------------------------

		ETInlineHint PhysicsAsset::RigidShape ParseShape(PxPhysics& physx, BoneIndex boneIndex, PxMaterial& material, const SphereShape* shape) {
			return { physx.createShape(PxSphereGeometry(shape->Radius()), material, /*isExclusive =*/false), boneIndex };
		}

		// ---------------------------------------------------

		ETInlineHint PhysicsAsset::RigidShape ParseShape(PxPhysics& physx, BoneIndex boneIndex, PxMaterial& material, const BoxShape* shape) {
			return { physx.createShape(PxBoxGeometry(AsPxVec3(shape->HalfExtent())), material, /*isExclusive =*/false), boneIndex };
		}

		// ---------------------------------------------------

		ETInlineHint PhysicsAsset::RigidShape ParseShape(PxPhysics& physx, BoneIndex boneIndex, PxMaterial& material, const CapsuleShape* shape) {
			return { physx.createShape(PxCapsuleGeometry(shape->Radius(), shape->Height() * 0.5f), material), boneIndex };
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode CreateRigidShapes(ArrayList<PhysicsAsset::RigidShape>& shapes, PxPhysics& physx, const flatbuffers::Vector<Offset<RigidBody>>* bodies) {
			for (uoffset_t id(0u); id < bodies->size(); ++id) {
				const RigidBody*         body(bodies->Get(id));
				PhysxPointer<PxMaterial> material(physx.createMaterial(body->Friction(), body->RollingFriction(), body->Restitution()));
				if (!material) {
					return Error::Unspecified;
				}

				switch (body->Shape_type()) {
				case CollisionShape::NONE: break;
				case CollisionShape::SphereShape: shapes.Append(ParseShape(physx, body->BoneIndex(), *material, body->Shape_as<SphereShape>())); break;
				case CollisionShape::BoxShape: shapes.Append(ParseShape(physx, body->BoneIndex(), *material, body->Shape_as<BoxShape>())); break;
				case CollisionShape::CapsuleShape: shapes.Append(ParseShape(physx, body->BoneIndex(), *material, body->Shape_as<CapsuleShape>())); break;
				//	Note that since we're reading from a disk asset, we *cannot* assume that the input value will always correspond to a known enumeration value.
				default: { return Error::InvalidParameter; }
				};
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ErrorCode CreateClothShapes(ArrayList<PhysicsAsset::ClothShape>& shapes, PxPhysics& /*physx*/, const flatbuffers::Vector<Offset<ClothBody>>* bodies) {
			for (uoffset_t id(0u); id < bodies->size(); ++id) {
				shapes.EmplaceBack();
			}

			//	Transfer ownership to the caller.
			return Error::None;
		}

	} // anonymous namespace

	PhysicsAsset::RigidShape::RigidShape(PhysxPointer<PxShape> shape, BoneIndex linkIndex) :
		shape(eastl::move(shape)),
		linkIndex(linkIndex) {}

	// ---------------------------------------------------

	PhysicsAsset::PhysicsAsset(StringView path) :
		Asset(path),
		_rigidShapes(MallocAllocator("PhysX Physics Asset Rigid Shape Collection Allocator")),
		_clothShapes(MallocAllocator("PhysX Physics Asset Cloth Shape Collection Allocator")) {
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

	ErrorCode PhysicsAsset::BindResources(const Builder& asset) {
		//	Verify the data we are considering can plausibly represent a collision asset.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyPhysicsBodyBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const PhysicsBody*    body(GetPhysicsBody(asset.Begin()));
		ArrayList<RigidShape> rigidShapes(_rigidShapes.GetAllocator());
		if (Failed(CreateRigidShapes(rigidShapes, PxGetPhysics(), body->RigidShapes()))) {
			asset.WriteLog(Severity::Error, "Rigid shape data for {} is malformed!" ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		ArrayList<ClothShape> clothShapes(_clothShapes.GetAllocator());
		if (Failed(CreateClothShapes(clothShapes, PxGetPhysics(), body->ClothShapes()))) {
			asset.WriteLog(Severity::Error, "Cloth shape data for {} is malformed!" ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		Swap(_rigidShapes, rigidShapes);
		Swap(_clothShapes, clothShapes);

		return Error::None;
	}

	// ---------------------------------------------------

	void PhysicsAsset::FreeResources() {
		_rigidShapes.Clear(ReleaseMemorySemantics());
		_clothShapes.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView PhysicsAsset::GetExtension() ETNoexceptHint {
		return { PhysicsBodyExtension(), StringLength(PhysicsBodyExtension()) };
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
