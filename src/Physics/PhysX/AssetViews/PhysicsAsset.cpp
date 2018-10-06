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
#include <Flatbuffers/FlatbufferTools.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/PhysicsBody_generated.h>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
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

		ETInlineHint ETForceInlineHint ETPureFunctionHint PxVec3 AsPxVec3(const Float3* vector) ETNoexceptHint {
			return PxVec3 { vector->X(), vector->Y(), vector->Z() };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint PhysicsAsset::RigidShape ParseShape(BoneIndex bone, PhysxPointer<PxMaterial> material, const SphereShape* shape) ETNoexceptHint {
			return { PxGetPhysics().createShape(PxSphereGeometry(shape->Radius()), *material, /*isExclusive =*/false), bone };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint PhysicsAsset::RigidShape ParseShape(BoneIndex bone, PhysxPointer<PxMaterial> material, const BoxShape* shape) ETNoexceptHint {
			return { PxGetPhysics().createShape(PxBoxGeometry(AsPxVec3(shape->HalfExtent())), *material, /*isExclusive =*/false), bone };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint PhysicsAsset::RigidShape ParseShape(BoneIndex bone, PhysxPointer<PxMaterial> material, const CapsuleShape* shape) ETNoexceptHint {
			return { PxGetPhysics().createShape(PxCapsuleGeometry(shape->Radius(), shape->Height() * 0.5f), *material), bone };
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode CreateRigidShapes(ArrayList<PhysicsAsset::RigidShape>& shapes, const FlatbufferVector<Offset<ShapeDescriptor>>* descriptors) {
			for (uoffset_t id(0u); id < descriptors->size(); ++id) {
				const ShapeDescriptor*   descriptor(descriptors->Get(id));
				PhysxPointer<PxMaterial> material(PxGetPhysics().createMaterial(descriptor->Friction(), descriptor->RollingFriction(), descriptor->Restitution()));
				if (!material) {
					return Error::Unspecified;
				}

				switch (descriptor->Shape_type()) {
				default:
				case CollisionShape::NONE: break;
				case CollisionShape::SphereShape: shapes.Append(ParseShape(descriptor->BoneIndex(), eastl::move(material), descriptor->Shape_as<SphereShape>())); break;
				case CollisionShape::BoxShape: shapes.Append(ParseShape(descriptor->BoneIndex(), eastl::move(material), descriptor->Shape_as<BoxShape>())); break;
				case CollisionShape::CapsuleShape: shapes.Append(ParseShape(descriptor->BoneIndex(), eastl::move(material), descriptor->Shape_as<CapsuleShape>())); break;
				};
			}

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode CreateClothShapes(ArrayList<PhysicsAsset::ClothShape>& shapes, const FlatbufferVector<Offset<ClothDescriptor>>* bodies) {
			for (uoffset_t id(0u); id < bodies->size(); ++id) {
				const ClothDescriptor* body(bodies->Get(id));
				// PxGetPhysics().createClothFabric();
			}

			return Error::None;
		}

	} // anonymous namespace

	PhysicsAsset::RigidShape::RigidShape(PhysxPointer<PxShape> shape, BoneIndex bone) :
		shape(eastl::move(shape)),
		bone(bone) {}

	// ---------------------------------------------------

	PhysicsAsset::ClothShape::ClothShape(PhysxPointer<PxClothFabric> fabric, ArrayList<PxClothParticle> particles, BoneIndex bone) :
		fabric(eastl::move(fabric)),
		particles(eastl::move(particles)),
		bone(bone) {}

	// ---------------------------------------------------

	PhysicsAsset::PhysicsAsset(StringView path) :
		Asset(path),
		_rigidShapes(MallocAllocator("PhysX Physics Asset Rigid Shape List Allocator")),
		_clothShapes(MallocAllocator("PhysX Physics Asset Cloth Shape List Allocator")) {
	}

	// ---------------------------------------------------

	Range<const PhysicsAsset::RigidShape*> PhysicsAsset::GetRigidShapes() const ETNoexceptHint {
		return { _rigidShapes.Begin(), _rigidShapes.End() };
	}

	// ---------------------------------------------------

	Range<const PhysicsAsset::ClothShape*> PhysicsAsset::GetClothShapes() const ETNoexceptHint {
		return { _clothShapes.Begin(), _clothShapes.End() };
	}

	// ---------------------------------------------------

	ErrorCode PhysicsAsset::BindResources(const Builder& asset) {
		//	Verify the data we are considering can plausibly represent a collision asset.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyPhysicsDescriptorBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const PhysicsDescriptor* descriptor(GetPhysicsDescriptor(asset.Begin()));
		ArrayList<RigidShape>    rigidShapes(_rigidShapes.GetAllocator(), descriptor->Shapes()->size());
		if (Failed(CreateRigidShapes(rigidShapes, descriptor->Shapes()))) {
			asset.WriteLog(Severity::Error, "Shape data for {} is malformed!" ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		ArrayList<ClothShape> clothShapes(_clothShapes.GetAllocator(), descriptor->ClothShapes()->size());
		if (Failed(CreateClothShapes(clothShapes, descriptor->ClothShapes()))) {
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
		return { PhysicsDescriptorExtension(), StringLength(PhysicsDescriptorExtension()) };
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
